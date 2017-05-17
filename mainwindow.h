#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "process.h"
#include "proctablemodel.h"
#include <QStandardItemModel>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
private:
    int n;
    process ** proc;
    int * pids;
    pthread_t mainthread;
    ProcTableModel * model;

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void printTable();
    void updateTable();
    static void* work(void*);

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
