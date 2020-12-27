#ifndef __snglplotdiag__
#define __snglplotdiag__

#include <QDialog>
#include <QCheckBox>
#include <QListWidget>
#include <vector>
#include <QtSql>
#include "queryMaster.hpp"
#include "plotter.hpp"

QT_BEGIN_NAMESPACE
namespace Ui {
class snglplotdiag;
}
QT_END_NAMESPACE

class snglplotdiag : public QDialog
{
    Q_OBJECT

public:
    snglplotdiag(QWidget *parent = nullptr,QueryMaster* qm=nullptr);
    ~snglplotdiag();
    QueryMaster* qm;
    void load_interested_range_(int start_time, int end_time, std::vector<QString> available_var);

private slots:

    void on_statistic_var_currentIndexChanged(const QString &arg1);
    void on_buttonBox_accepted();

public slots:
    void query_to_range_(QSqlQuery* q,QueryMaster* sender);
signals:
    void query_(QString q);
    void plot_(QString var, int from, int to,double startvalue, double endvalue, int gridnum, double step,bool sumcnt, bool accumul, bool smooth, PlotTypeEnum graphtype);
private:
    Ui::snglplotdiag *ui;
};

#endif
