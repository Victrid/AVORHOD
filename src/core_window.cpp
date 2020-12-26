#include "core_window.h"
#include "./ui_core_window.h"

#include "csvdiag.h"
#include "queryMaster.hpp"

#include <QPushButton>
#include <QtCharts/QtCharts>

using namespace std;

core_window::core_window(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::core_window)
{
    using namespace Qt;
    ui->setupUi(this);
    ui->statusbar->addPermanentWidget(statusbar_progressbar_);
    ui->statusbar->addPermanentWidget(statusbar_text_);
    connect(&queryMaster_,&QueryMaster::change_statusbar_,this,&core_window::update_statusbar_);
    connect(&queryMaster_,&QueryMaster::database_load_finished_,this,&core_window::unlock_window_);
    queryMaster_.comm();
}

core_window::~core_window()
{
    delete ui;
}

void core_window::update_statusbar_(QString message, int current, int total){
    statusbar_text_->setText(message);
    statusbar_progressbar_->setMinimum(0);
    statusbar_progressbar_->setMaximum(total);
    statusbar_progressbar_->setValue(current);
}

void core_window::unlock_window_(bool unlock){
    ui->tabWidget->setEnabled(unlock);
}

void core_window::set_parameters_(int a, int b, std::vector<bool> c){
    interested_time_lower_limit_=a;
    interested_time_upper_limit_=b;
    interested_fields_=c;
}


void core_window::on_actionLoad_dataset_triggered()
{
    csvdiag* diag = new csvdiag();
    connect(diag,&csvdiag::path_is_set_,&queryMaster_,&QueryMaster::load_database_);
    connect(diag,&csvdiag::parameter_is_set_,this,&core_window::set_parameters_);
    diag->show();
}

void core_window::on_actionQuit_triggered()
{
    close();
}

void core_window::on_actionX_plot_triggered()
{
    snglplotdiag* diag = new snglplotdiag(nullptr,&queryMaster_);
    vector<QString> qs;
    for(unsigned long i=1;i<interested_fields_.size();i++)
        if(interested_fields_[i])
            qs.push_back(data_keyword[i]);
    diag->load_interested_range_(interested_time_lower_limit_,interested_time_upper_limit_,qs);
    diag->show();
}
