#include "process.h"
#include "mainwindow.h"
#include <stdio.h>
#include <QApplication>

int main(int argc, char *argv[])
{

    QApplication a(argc, argv);
    CubeMainWindow m;
    m.show();
    return a.exec();
}
