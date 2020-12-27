#ifndef __plotter__
#define __plotter__

#include <QCoreApplication>
#include <QtSql>
#include <fstream>
#include <string>
#include <stdexcept>
#include <QtCharts/QtCharts>
#include <queue>

#include "queryMaster.hpp"

enum PlotTypeEnum {linechart,histogram,piechart};

class Plotter:public QObject {
private:
    Q_OBJECT
public:

    QueryMaster* qm;
    struct par{
        bool accumul; bool smooth; PlotTypeEnum graphtype;
    };
    std::queue<par> plotq;
    QString build_statistical_query(QString var, int from, int to,double startv, double endv, int gridnum, double step,bool sumcnt);
    QString build_timal_query(QString var, int from, int to,double startv, double endv, int gridnum, double step,bool sumcnt);
    void linechart_plot(QLineSeries* s,bool accumul,bool smooth);
    void linechart_plot(std::vector<double>& x,std::vector<double>& y,bool accumul,bool smooth);
    void histogram_plot(QBarSet* s,bool accumul,bool smooth);
    void histogram_plot(std::vector<double>&,std::vector<double>& y,bool accumul,bool smooth);
    void piechart_plot(QPieSeries* s,bool accumul,bool smooth);
    void piechart_plot(std::vector<double>& x,std::vector<double>& y,bool accumul,bool smooth);

    Plotter(QueryMaster* qm);;

public slots:
    void statistic_plots_query_(QString var, int from, int to,double startvalue, double endvalue, int gridnum, double step,bool sumcnt, bool accumul, bool smooth, PlotTypeEnum graphtype);
    void timal_plots_query_(QString var, int from, int to,double startvalue, double endvalue, int gridnum, double step,bool sumcnt, bool accumul, bool smooth, PlotTypeEnum graphtype);
    void query_to_plot_(QSqlQuery *q, QueryMaster *sender);

signals:
    void query_(QString var);
    void plot_to_canvas(QChart*, bool);
};

#endif
