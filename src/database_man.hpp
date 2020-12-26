#ifndef __database_manipulator__
#define __database_manipulator__

#include <QCoreApplication>
#include <QtSql>
#include <stdexcept>
#include <string>
#include <vector>
#include <deque>

#include "database.hpp"

class DatabaseManipulator :public QObject{
private:
    Q_OBJECT

    Database* db;
public slots:
    void load_database_(QString directory,QString grid);
    void query_(QString qwords);
signals:
    void change_statusbar_(QString q, int current, int total);
    void database_loaded_(int type);
    void query_done_(QSqlQuery* q);
};

#endif
