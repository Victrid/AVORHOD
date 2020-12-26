#include "queryMaster.hpp"
QueryMaster::QueryMaster() {
    DatabaseManipulator *dl = new DatabaseManipulator();
    dl->moveToThread(&query_thread_);
    connect(&query_thread_, &QThread::finished, dl, &QObject::deleteLater);
    connect(this, &QueryMaster::database_load_start_, dl, &DatabaseManipulator::load_database_);
    //connect(this, &queryMaster::startquery, dl, &dataLoader::query);
    connect(dl,&DatabaseManipulator::change_statusbar_,this, &QueryMaster::change_statusbar_);
    connect(dl,&DatabaseManipulator::database_loaded_,this,&QueryMaster::signal_update_complete_);
    connect(this,&QueryMaster::query_,dl,&DatabaseManipulator::query_);
    connect(dl,&DatabaseManipulator::query_done_,this,&QueryMaster::signal_query_done_);
    query_thread_.start();
}

QueryMaster::~QueryMaster(){
    query_thread_.terminate();
}

void QueryMaster::comm(){
    emit change_statusbar_("Database connection established.",100,100);
}

void QueryMaster::signal_update_complete_(int type){
    QString vq[] = {"Loading complete","Query complete","ROI process complete"};
    emit change_statusbar_(vq[type],100,100);
    if(type == 0)
        emit database_load_finished_(true);
}

void QueryMaster::load_database_(QString directory, QString grid){
    emit database_load_finished_(false);
    emit change_statusbar_("Data loading in progress",0,100);
    emit database_load_start_(directory,grid);
}

void QueryMaster::query_(QString qwords){
    emit change_statusbar_("Querying in progress",0,100);
    emit query_(qwords);
}

void QueryMaster::signal_query_done_(QSqlQuery *q){
    emit change_statusbar_("Query Complete",100,100);
    emit query_finished_(q,this);
}
