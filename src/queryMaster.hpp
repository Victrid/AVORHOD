#ifndef __dataops__
#define __dataops__

#include <QCoreApplication>
#include <QtSql>
#include <stdexcept>
#include <string>
#include <vector>
#include <deque>

#include "database.hpp"
#include "database_man.hpp"

class QueryMaster : public QObject{
private:
    Q_OBJECT

    QThread query_thread_;
    Database* database_;
    bool loading_finished = false;
public:
    QueryMaster();
    ~QueryMaster();
    void comm();
public slots:
    void load_database_(QString directory, QString grid);
    void query_(QString qwords);
    void signal_query_done_(QSqlQuery* q);

    void signal_update_complete_(int type);


signals:
    void change_statusbar_(QString info, int current, int total);

    void query_start_(QString);
    void query_finished_(QSqlQuery* query,QueryMaster* obj);

    void database_load_start_(QString directory, QString grid);
    void database_load_finished_(bool);
};

#endif
