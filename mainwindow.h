#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "process.h"
#include "proctablemodel.h"
#include <QStandardItemModel>

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

public:
    explicit CubeMainWindow(QWidget *parent = 0);
    ~CubeMainWindow();
    void printTable();
    void updateTable();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

public slots:
    void rehreshTable();


private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
