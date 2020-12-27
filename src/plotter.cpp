#include "plotter.hpp"

using namespace std;

QString Plotter::build_statistical_query(QString var, int from, int to, double startv, double endv, int gridnum, double step,bool sumcnt){
    using namespace std;
    string str= " select "+var.toStdString()+", "+(sumcnt?"sum":"count")+"("+var.toStdString()+") "
        " from _data "
        " where "
        " "+var.toStdString()+">="+to_string(startv)+" and "+var.toStdString()+"<="+to_string(endv)+" "
        " and departure_time>= "+to_string(from)+" and end_time<= "+to_string(to)+" "
        " and orig_lng >= (select vertex3_lng from _grid where grid_id= "+to_string(gridnum)+" ) and orig_lng<=(select vertex1_lng from _grid where grid_id= "+to_string(gridnum)+" ) "
        " and orig_lat >= (select vertex3_lat from _grid where grid_id= "+to_string(gridnum)+" ) and orig_lat<=(select vertex1_lat from _grid where grid_id= "+to_string(gridnum)+" ) "
        " group by cast( "+var.toStdString()+" / "+to_string(step)+" as int ) * "+to_string(step)+" ;";
    return QString::fromStdString(str);
}

QString Plotter::build_timal_query(QString var, int from, int to, double startv, double endv, int gridnum, double step,bool sumcnt){
    using namespace std;
    string str="";
    str = str +" select departure_time, "+(sumcnt? "sum" : "count")+"("+var.toStdString()+") "
        " from _data "
        " where "
        " "+var.toStdString()+">="+to_string(startv)+" and "+var.toStdString()+"<="+to_string(endv)+" "
        " and departure_time>= "+to_string(from)+" and end_time<= "+to_string(to)+" "
        " and orig_lng >= (select vertex3_lng from _grid where grid_id= "+to_string(gridnum)+" ) and orig_lng<=(select vertex1_lng from _grid where grid_id= "+to_string(gridnum)+" ) "
        " and orig_lat >= (select vertex3_lat from _grid where grid_id= "+to_string(gridnum)+" ) and orig_lat<=(select vertex1_lat from _grid where grid_id= "+to_string(gridnum)+" ) "
        " group by cast( departure_time / "+to_string(step)+" as int ) * "+to_string(step)+" ;";
    return QString::fromStdString(str);
}

void Plotter::linechart_plot(QLineSeries* s, bool accumul, bool smooth){
    QLineSeries *qs=new QLineSeries();
    QChart* qc = new QChart();

    if(s->count()==0)return;
    if(!smooth){
        if(accumul){
            *qs << s->at(0);
            QPointF qf = s->at(0);
            for(int i=1;i<s->count();i++){
                qf.setY(qf.y()+s->at(i).y());
                qf.setX(s->at(i).x());
                *qs << qf;
            }
            qc->addSeries(qs);
        }else{
            qc->addSeries(s);
        }
    }else{
        QSplineSeries* qss = new QSplineSeries();
        if(accumul){
            *qss << s->at(0);
            QPointF qf = s->at(0);
            for(int i=1;i<s->count();i++){
                qf.setY(qf.y()+s->at(i).y());
                qf.setX(s->at(i).x());
                *qss << qf;
            }
        }else{
            for(int i=0;i<s->count();i++)
                *qss << s->at(i);
        }
        qc->addSeries(qss);
    }

    qc->createDefaultAxes();
    emit plot_to_canvas(qc,smooth);
}

void Plotter::linechart_plot(std::vector<double>& x, std::vector<double>& y, bool acccumul, bool smooth){
    QLineSeries* s=new QLineSeries();
    for(auto a=x.begin(),b=y.begin();a!=x.end()&&b!=y.end();a++,b++){
        s->append(*a,*b);
    }
    linechart_plot(s,acccumul,smooth);
}

void Plotter::histogram_plot(QBarSet* s, bool accumul, bool smooth){
    QBarSet *qs=new QBarSet(s->label());
    QBarSeries *qbs = new QBarSeries();
    if(s->count()==0)return;
    if(accumul){
        qs->insert(0,s->at(0));
        for(int i=1;i<s->count();i++){
            qs->insert(i,s->at(i)+qs->at(i-1));
        }
        qbs->append(qs);
    }else{
        qbs->append(s);
    }
    QChart* qc = new QChart();
    qc->addSeries(qbs);
    qc->createDefaultAxes();
    emit plot_to_canvas(qc,smooth);
}

void Plotter::histogram_plot(std::vector<double> &, std::vector<double> &y, bool accumul, bool smooth){
    QBarSet* qbs= new QBarSet("Value");
    for(auto i:y){
        qbs->append(i);
    }
    histogram_plot(qbs,accumul,smooth);
}

void Plotter::piechart_plot(QPieSeries* s, bool, bool smooth){
    QChart* qc = new QChart();
    qc->addSeries(s);
    emit plot_to_canvas(qc,smooth);
}

void Plotter::piechart_plot(std::vector<double> &x, std::vector<double> &y, bool accumul, bool smooth){
    QPieSeries* qps = new QPieSeries();
    for(auto a=x.begin(),b=y.begin();a!=x.end()&&b!=y.end();a++,b++){
        QString qf;
        qf.setNum(*a);
        qf.append("-");
        if(a+1==x.end()){
            qf.append("max");
        }else{
            QString qs;
            qs.setNum(*(a+1));
            qf.append(qs);
        }
        qps->append(qf,*b);
    }
    piechart_plot(qps,accumul,smooth);
}

Plotter::Plotter(QueryMaster *qm):qm(qm){
    connect(this,&Plotter::query_,qm,&QueryMaster::query_);
}

void Plotter::statistic_plots_query_(QString var, int from, int to, double startvalue, double endvalue, int gridnum, double step, bool sumcnt, bool accumul, bool smooth, PlotTypeEnum graphtype){
    plotq.push(par{accumul,smooth,graphtype});
    connect(qm,&QueryMaster::query_finished_,this,&Plotter::query_to_plot_);
    emit query_(build_statistical_query(var,from,to,startvalue,endvalue,gridnum,step,sumcnt));
}

void Plotter::timal_plots_query_(QString var, int from, int to, double startvalue, double endvalue, int gridnum, double step, bool sumcnt, bool accumul, bool smooth, PlotTypeEnum graphtype){
    plotq.push(par{accumul,smooth,graphtype});
    connect(qm,&QueryMaster::query_finished_,this,&Plotter::query_to_plot_);
    emit query_(build_timal_query(var,from,to,startvalue,endvalue,gridnum,step,sumcnt));
}

void Plotter::query_to_plot_(QSqlQuery *q, QueryMaster *sender){
    disconnect(sender,&QueryMaster::query_finished_,this,&Plotter::query_to_plot_);
    std::vector<double> x,y;
    while(q->next()){
        x.push_back(q->value(0).toFloat());
        y.push_back(q->value(1).toFloat());
    }
    par p = plotq.front();
    plotq.pop();
    switch (p.graphtype) {
    case linechart:
        linechart_plot(x,y,p.accumul,p.smooth);
        break;
    case histogram:
        histogram_plot(x,y,p.accumul,p.smooth);
        break;
    case piechart:
        piechart_plot(x,y,p.accumul,p.smooth);
        break;
    }
}
