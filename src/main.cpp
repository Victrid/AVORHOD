#include "core_window.h"
#include "queryMaster.hpp"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    core_window w;
    w.show();
    return a.exec();
}
