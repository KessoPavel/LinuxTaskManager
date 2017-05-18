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
    connect(this->ui->tableView->horizontalHeader(),SIGNAL(sectionClicked(int)),this,SLOT(sortTable(int)));


    this->rehreshThread->start();

    //pthread_create(&this->mainthread,NULL,rehreshThread,this);
}

CubeMainWindow::~CubeMainWindow() {
    delete ui;
}

void CubeMainWindow::printTable() {

    for (int i = 0; i < 8; i++){
       if (this->sotrFlag[i] == 1){
           sort(this->proc,this->n,i,this->flags[i]);
       }
    }

    model = new  ProcTableModel(this->proc, this->n, NULL);
    ui->tableView->setModel(model);

    ui->tableView->verticalHeader()->hide();

    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableView->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableView->setAutoScroll(false);
    ui->tableView->horizontalHeader()->setSectionResizeMode(0,QHeaderView::Stretch);
}





void CubeMainWindow::on_pushButton_clicked() {
    QModelIndex ind = this->ui->tableView->currentIndex();
    //TODO pid from table!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!11
    kill(this->pids[ind.row()],SIGKILL);
    this->updateTable();
}

void CubeMainWindow::updateTable() {

    QModelIndex ind = this->ui->tableView->currentIndex();
    ProcTableModel * model = (ProcTableModel *)this->ui->tableView->model();
    int pid = model->getData(ind.row(),1).toInt();

    this->pids = getPids(&n);
    this->proc = getProcess(pids, n);

    this->printTable();

    int nPid = getPidNumber(this->proc, this->n, pid);
    //получить номер столюца с пидом
    //либо поиск в массиве пидов
    //либо поиск в моделе


    this->ui->tableView->setCurrentIndex(this->ui->tableView->model()->index(nPid,2));

    if (nPid != -1){
        this->ui->tableView->setFocus();
    }
}

void CubeMainWindow::on_pushButton_2_clicked() {
    this->updateTable();
}

void CubeMainWindow::rehreshTable(){
    this->updateTable();
}

void CubeMainWindow::sortTable(int coll){
    this->flags[coll] *= -1;
    for (int i = 0; i < 8; i++){
        this->sotrFlag[i] = 0;
    }
    this->sotrFlag[coll] = 1;
    sort(this->proc,this->n, coll, this->flags[coll]);

}
