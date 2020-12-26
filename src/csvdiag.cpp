#include "csvdiag.h"
#include "./ui_csvdiag.h"

#include <QFileDialog>
#include <QDir>
#include <QFileInfo>

void csvdiag::load_interesting_field_checkbox_(){
    QStringList strList;
    strList << "order_id"<<"departure_time"<<"end_time"<<"orig_lng"<<"orig_lat"<<"dest_lng"<<"dest_lat"<<"fee";
    ui->listcheckbox->clear();
    ui->listcheckbox->addItems(strList);
    QListWidgetItem* item;
    for(int i = 0; i < ui->listcheckbox->count(); ++i){
        item = ui->listcheckbox->item(i);
        item->setFlags(item->flags() | Qt::ItemIsUserCheckable);
        item->setCheckState(Qt::Checked);
    }
    ui->listcheckbox->setEnabled(true);
}

csvdiag::csvdiag(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::csvdiag) {
    ui->setupUi(this);
}

csvdiag::~csvdiag() {
    delete ui;
}

void csvdiag::on_openfolder_clicked()
{
    QString path = QFileDialog::getExistingDirectory(this,"Select CSV folder");
    if(path=="")return;
    QDir dir(path);
    if(!dir.exists())return;
    ui->csvpath->setText(dir.absolutePath());
    QFileInfo csvfile(dir.filePath("rectangle_grid_table.csv"));
    ui->opengridcsv->setEnabled(true);
    if(csvfile.exists()){
        ui->gridpath->setText(csvfile.absoluteFilePath());
        emit path_is_set_(ui->csvpath->text(),ui->gridpath->text());
        ui->buttonBox->setEnabled(true);
        load_interesting_field_checkbox_();
    }
}

void csvdiag::on_opengridcsv_clicked()
{
    QString path = QFileDialog::getOpenFileName(this,"Select grid CSV file",ui->csvpath->text());
    QFileInfo csvfile(path);
    if(csvfile.exists()){
        ui->gridpath->setText(csvfile.absoluteFilePath());
        emit path_is_set_(ui->csvpath->text(),ui->gridpath->text());
        ui->buttonBox->setEnabled(true);
    }
    load_interesting_field_checkbox_();
}

void csvdiag::on_buttonBox_accepted()
{
    int tstart = ui->time_start->dateTime().toTime_t();
    int tend = ui->time_end->dateTime().toTime_t();
    std::vector<bool> selected;
    for(int i = 0; i < ui->listcheckbox->count(); ++i)
    {
        QListWidgetItem* item = ui->listcheckbox->item(i);
        selected.push_back(item->checkState());
    }
    emit parameter_is_set_(tstart,tend,selected);
    this->close();
}
