#include "database.hpp"

using namespace std;

bool Database::load_csv_to_data_(string filename) {
    bool retval = true;
    fstream fs(filename);
    if (!fs)
        throw invalid_argument("E0x03");
    string str;
    getline(fs, str);
    QSqlQuery query;
    while (getline(fs, str))
        retval &= query.exec(QString::fromStdString(
            "INSERT INTO _data VALUES (\"" + str.substr(0, 31) + '\"' +
            str.substr(32) + ");"));
    return retval;
}

bool Database::load_csv_to_grid_(string filename) {
    bool retval = true;
    fstream fs(filename);
    if (!fs)
        throw invalid_argument("E0x03");
    string str;
    getline(fs, str);
    QSqlQuery query;
    while (getline(fs, str))
        retval &= query.exec(
            QString::fromStdString("INSERT INTO _grid VALUES (" + str + ");"));
    return retval;
}

Database::Database() {
    database_ = QSqlDatabase::addDatabase("QSQLITE");
    database_.setDatabaseName("data.db");
    if (!database_.open())
        throw invalid_argument("E0x01");
    query_ = QSqlQuery(database_);
    return;
}

void Database::init_database_() {
    query_.exec(
        "begin;"
        "DROP TABLE IF EXISTS _data;"
        "DROP TABLE IF EXISTS _grid;"
        "CREATE TABLE _data(order_id text primary key,departure_time "
        "double,end_time double ,orig_lng double ,orig_lat double "
        ",dest_lng double ,dest_lat double, fee double );"
        "CREATE TABLE _grid(grid_id integer primary key, vertex0_lng "
        "double,vertex0_lat double,vertex1_lng double,vertex1_lat "
        "double,vertex2_lng double,vertex2_lat double,vertex3_lng "
        "double,vertex3_lat double);"
        "commit;");
}

Database::~Database() { database_.close(); }
