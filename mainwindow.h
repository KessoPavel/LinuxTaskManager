#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "process.h"
#include "proctablemodel.h"
#include "logic.h"
#include "processorlogic.h"
#include <QStandardItemModel>
#include <QLabel>
#include <QProgressBar>

class RehreshThread;

namespace Ui {
class MainWindow;
}

class CubeMainWindow : public QMainWindow
{
    Q_OBJECT
private:
    RehreshThread * rehreshThread;
    int n;
    process ** proc;
    int * pids;
    pthread_t mainthread;
    ProcTableModel * model;
    int flags[8];
    int sotrFlag[8];
    USER_FILTER userFilter;
    STATE_FILTER stateFilter;
    char * nfilter;
    CPU * cpu_used;

    QLabel ** cpux;
    QProgressBar ** cpuxP;


public:
    explicit CubeMainWindow(QWidget *parent = 0);
    ~CubeMainWindow();
    void printTable();
    void updateTable();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_lineEdit_textChanged(const QString &arg1);

public slots:
    void rehreshTable();

    void sortTable(int);

    void setMyProcessFilter(bool);
    void setRootProcessFilter(bool);
    void setAllProcessFilter(bool);

    void setSleepingStareFilter(bool);
    void setRunningStateFilter(bool);
    void setAllStateFilter(bool);

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
