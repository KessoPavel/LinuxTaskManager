#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "logic.h"
#include <signal.h>
#include <unistd.h>
#include <pthread.h>
#include "rehreshthread.h"

CubeMainWindow::CubeMainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow) {
    ui->setupUi(this);
    this->pids = getPids(&n);
    this->proc = getProcess(pids, n);

    this->printTable();

    this->rehreshThread = new RehreshThread(this);

    connect(this->rehreshThread,SIGNAL(rehresh()),this,SLOT(rehreshTable()));


    this->rehreshThread->start();

    //pthread_create(&this->mainthread,NULL,rehreshThread,this);
}

CubeMainWindow::~CubeMainWindow() {
    delete ui;
}

void CubeMainWindow::printTable() {
    model = new  ProcTableModel(this->proc, this->n, NULL);
    ui->tableView->setModel(model);

    ui->tableView->verticalHeader()->hide();

    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableView->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
}





void CubeMainWindow::on_pushButton_clicked() {
    QModelIndex ind = this->ui->tableView->currentIndex();
    kill(this->pids[ind.row()],SIGKILL);
}

void CubeMainWindow::updateTable() {

    QModelIndex ind = this->ui->tableView->currentIndex();
    ProcTableModel * model = (ProcTableModel *)this->ui->tableView->model();
    int pid = model->getData(ind.row(),1).toInt();

    this->pids = getPids(&n);
    this->proc = getProcess(pids, n);

    this->printTable();

    //получить номер столюца с пидом
    //либо поиск в массиве пидов
    //либо поиск в моделе

    this->ui->tableView->setCurrentIndex(this->ui->tableView->model()->index(2,2));//TODO
    this->ui->tableView->setFocus();
}

void CubeMainWindow::on_pushButton_2_clicked() {
    this->updateTable();
}

void CubeMainWindow::rehreshTable(){
    //this->updateTable();
}
