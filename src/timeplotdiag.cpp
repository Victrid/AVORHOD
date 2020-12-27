#include "timeplotdiag.h"
#include "./ui_timeplotdiag.h"

#include <QFileDialog>

timeplotdiag::timeplotdiag(QWidget *parent,QueryMaster* qm) :
    QDialog(parent), qm(qm),
    ui(new Ui::timeplotdiag){
    ui->setupUi(this);
    connect(this,&timeplotdiag::query_,qm,&QueryMaster::query_);
    //TODO var dep on grid value;
    QString qs;
    for(int i=0;i<=99;i++){
        qs.setNum(i);
        ui->gridnum->addItem(qs);
    }
}

timeplotdiag::~timeplotdiag() {
    delete ui;
}

void timeplotdiag::load_interested_range_(int start_time, int end_time, std::vector<QString> available_var){
    //Start and end time
    QDateTime t;
    t.setTime_t(start_time);
    ui->timestart->setMinimumDateTime(t);
    ui->timestart->setDateTime(t);
    t.setTime_t(end_time);
    ui->timeend->setMaximumDateTime(t);
    ui->timeend->setDateTime(t);
    //variable
    for(auto& t: available_var){
        ui->statistic_var->addItem(t);
    }
    ui->statistic_var->setCurrentIndex(0);
    ui->stepvalue->setMaximum(end_time-start_time);
    ui->stepvalue->setValue((end_time-start_time)/100);
    //TODO variable range load
    QString queryminmax="select min("+ui->statistic_var->currentText()+"), max("+ui->statistic_var->currentText()+") from _data;";
    connect(qm,&QueryMaster::query_finished_,this,&timeplotdiag::query_to_range_);
    emit query_(queryminmax);
}

void timeplotdiag::query_to_range_(QSqlQuery *q, QueryMaster *sender){
    q->next();
    ui->Startvalue->setMinimum(q->value(0).toFloat());
    ui->Startvalue->setValue(q->value(0).toFloat());
    ui->Endvalue->setMaximum(q->value(1).toFloat());
    ui->Endvalue->setValue(q->value(1).toFloat());
    ui->Startvalue->setEnabled(true);
    ui->Endvalue->setEnabled(true);
    disconnect(sender,&QueryMaster::query_finished_,this,&timeplotdiag::query_to_range_);
}

void timeplotdiag::on_statistic_var_currentIndexChanged(const QString &)
{
    QString queryminmax="select min("+ui->statistic_var->currentText()+"), max("+ui->statistic_var->currentText()+") from _data;";
    connect(qm,&QueryMaster::query_finished_,this,&timeplotdiag::query_to_range_);
    emit query_(queryminmax);
}

void timeplotdiag::on_buttonBox_accepted()
{
    //
    PlotTypeEnum gtype;
    switch (ui->chart->currentIndex()) {
    case 0:
        gtype = PlotTypeEnum::piechart;
        break;
    case 1:
        gtype = PlotTypeEnum::histogram;
        break;
    case 2:
        gtype = PlotTypeEnum::linechart;
        break;
    }
    emit plot_(ui->statistic_var->currentText(),
              ui->timestart->dateTime().toTime_t(),
              ui->timeend->dateTime().toTime_t(),
              ui->Startvalue->value(),
              ui->Endvalue->value(),
              ui->gridnum->currentIndex(),
              ui->stepvalue->value(),
              ui->sumcnt->checkState()==Qt::Checked,
              ui->accdistr->checkState()==Qt::Checked,
              ui->smooth->checkState()==Qt::Checked,
              gtype);
}
