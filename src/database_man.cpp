#include "database_man.hpp"
#include <QtDebug>

void DatabaseManipulator::load_database_(QString directory, QString grid){
    bool retval = true;
    QDir dir(directory);
    QFileInfo gridinfo(grid);
    dir.setFilter(QDir::Files | QDir::Hidden | QDir::NoSymLinks);
    std::vector<std::string> dirlist;
    for (auto &file : dir.entryInfoList())
        if(gridinfo.absoluteFilePath()!=file.absoluteFilePath())
            dirlist.push_back(file.absoluteFilePath().toStdString());
    db = new Database();
    db->init_database_();
    int t = 0;
    db->query_.exec("begin;");
    for (auto &file : dirlist){
        retval &= db->load_csv_to_data_(file);
        t++;
        emit change_statusbar_("Loading data",t,dirlist.size());
    }
    db->query_.exec("commit;");
    db->query_.exec("begin;");
    db->load_csv_to_grid_(gridinfo.absoluteFilePath().toStdString());
    db->query_.exec("commit;");
    emit database_loaded_(0);
}

void DatabaseManipulator::query_(QString qwords){
    QSqlQuery* q = new QSqlQuery(db->database_);
    q->exec(qwords);
    emit query_done_(q);
}
