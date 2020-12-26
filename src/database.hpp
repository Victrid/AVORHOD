#ifndef DATABASE_HPP
#define DATABASE_HPP

#include <QCoreApplication>
#include <QtSql>
#include <fstream>
#include <string>
#include <stdexcept>

// It says Everything should be coded in C++ & Qt
// literally, QtSql is a component of Qt
// -> I can use QtSql
class Database {
public:
    QSqlDatabase database_;
    QSqlQuery query_;
    bool load_csv_to_data_(std::string filename);
    bool load_csv_to_grid_(std::string filename);
    Database();
    void init_database_();
    ~Database();
};

#endif // DATABASE_HPP
