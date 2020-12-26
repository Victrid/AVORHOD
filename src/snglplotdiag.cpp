#include "snglplotdiag.h"
#include "./ui_snglplotdiag.h"

#include <QFileDialog>

snglplotdiag::snglplotdiag(QWidget *parent,QueryMaster* qm) :
    QDialog(parent), qm(qm),
    ui(new Ui::snglplotdiag){
    ui->setupUi(this);
    connect(this,&snglplotdiag::query_,qm,&QueryMaster::query_);
    //TODO var dep on grid value;
    QString qs;
    for(int i=0;i<=99;i++){
        qs.setNum(i);
        ui->gridnum->addItem(qs);
    }
}

snglplotdiag::~snglplotdiag() {
    delete ui;
}

void snglplotdiag::load_interested_range_(int start_time, int end_time, std::vector<QString> available_var){
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
    //TODO variable range load
    QString queryminmax="select min("+ui->statistic_var->currentText()+"), max("+ui->statistic_var->currentText()+") from _data;";
    connect(qm,&QueryMaster::query_finished_,this,&snglplotdiag::query_to_range_);
    emit query_(queryminmax);
}

void snglplotdiag::query_to_range_(QSqlQuery *q, QueryMaster *sender){
    q->next();
    ui->Startvalue->setMinimum(q->value(0).toFloat());
    ui->Startvalue->setValue(q->value(0).toFloat());
    ui->Endvalue->setMaximum(q->value(1).toFloat());
    ui->Endvalue->setValue(q->value(1).toFloat());
    ui->stepvalue->setMaximum(q->value(1).toFloat()-q->value(0).toFloat());
    ui->Startvalue->setEnabled(true);
    ui->Endvalue->setEnabled(true);
    disconnect(sender,&QueryMaster::query_finished_,this,&snglplotdiag::query_to_range_);
}

void snglplotdiag::on_statistic_var_currentIndexChanged(const QString &)
{
    QString queryminmax="select min("+ui->statistic_var->currentText()+"), max("+ui->statistic_var->currentText()+") from _data;";
    connect(qm,&QueryMaster::query_finished_,this,&snglplotdiag::query_to_range_);
    emit query_(queryminmax);
}
