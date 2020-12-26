#ifndef __CSVDIAG__
#define __CSVDIAG__

#include <QDialog>
#include <QCheckBox>
#include "queryMaster.hpp"
#include <QListWidget>

namespace Ui {
class csvdiag;
}

class csvdiag : public QDialog
{
    Q_OBJECT
private:
    void load_interesting_field_checkbox_();

public:
    csvdiag(QWidget *parent = nullptr);
    ~csvdiag();

private slots:
    void on_openfolder_clicked();

    void on_opengridcsv_clicked();

    void on_buttonBox_accepted();

signals:
    void path_is_set_(QString directory, QString grid);
    void parameter_is_set_(int,int,std::vector<bool>);
private:
    Ui::csvdiag *ui;
};

#endif
