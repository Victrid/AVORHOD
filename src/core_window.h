#ifndef CORE_WINDOW_H
#define CORE_WINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QProgressBar>
#include "queryMaster.hpp"
#include "snglplotdiag.h"
#include "timeplotdiag.h"
#include <QtCharts/QtCharts>
#include "plotter.hpp"
#include <QGeoRoute>

QT_BEGIN_NAMESPACE
namespace Ui { class core_window; }
QT_END_NAMESPACE

const QString data_keyword[] = {"order_id","departure_time","end_time","orig_lng","orig_lat","dest_lng","dest_lat","fee"};

class core_window : public QMainWindow
{
    Q_OBJECT
private:
    QLabel *statusbar_text_ = new QLabel();
    QProgressBar * statusbar_progressbar_ = new QProgressBar();
    QueryMaster queryMaster_;
    int interested_time_lower_limit_;
    int interested_time_upper_limit_;
    std::vector<bool> interested_fields_;

public:
    core_window(QWidget *parent = nullptr);
    Plotter p;
    ~core_window();

public slots:
    void update_statusbar_(QString message,int current, int total);
    void unlock_window_(bool unlock);
    void set_parameters_(int a,int b,std::vector<bool> c);
    void plot(QChart* q, bool b);
private slots:

    void on_actionLoad_dataset_triggered();

    void on_actionQuit_triggered();

    void on_actionX_plot_triggered();

    void on_action2_var_plot_triggered();

private:
    Ui::core_window *ui;
};
#endif // CORE_WINDOW_H
