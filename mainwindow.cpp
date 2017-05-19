#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "logic.h"
#include <signal.h>
#include <unistd.h>
#include <pthread.h>
#include "processorlogic.h"
#include "rehreshthread.h"

CubeMainWindow::CubeMainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow) {

    init();
    processorInfinit();

    this->cpu_used = (CPU*)malloc(sizeof(CPU)*1);
    cpuInit(this->cpu_used);

    for(int i = 0; i < 8; i++){
        this->flags[i] = 1;
        this->sotrFlag[i] = 0;
    }
    this->userFilter = ALL_USERS;
    this->stateFilter = ALL;
    this->nfilter  = (char*)calloc(150,sizeof(char));

    ui->setupUi(this);
    this->pids = getPids(&n);
    this->proc = getProcess(pids, n);


    this->printTable();

    this->rehreshThread = new RehreshThread(this);

    connect(this->rehreshThread,SIGNAL(rehresh()),this,SLOT(rehreshTable()));
    //connect(this->rehreshThread,SIGNAL(rehresh()),this,SLOT(rehreshUsegeCPU()));
    connect(this->ui->tableView->horizontalHeader(),SIGNAL(sectionClicked(int)),this,SLOT(sortTable(int)));
    //filters
    connect(this->ui->MyProcess,SIGNAL(triggered(bool)),this,SLOT(setMyProcessFilter(bool)));
    connect(this->ui->RootProcess,SIGNAL(triggered(bool)),this,SLOT(setRootProcessFilter(bool)));
    connect(this->ui->Allprocess,SIGNAL(triggered(bool)),this,SLOT(setAllProcessFilter(bool)));
    connect(this->ui->SleepingProcess,SIGNAL(triggered(bool)),this,SLOT(setSleepingStareFilter(bool)));
    connect(this->ui->RunningProcess,SIGNAL(triggered(bool)),this,SLOT(setRunningStateFilter(bool)));
    connect(this->ui->AllProcess,SIGNAL(triggered(bool)),this,SLOT(setAllStateFilter(bool)));

    this->rehreshThread->start();
}

CubeMainWindow::~CubeMainWindow() {
    delete ui;
}

void CubeMainWindow::printTable() {
    int temp = this->n;
    this->proc = setFilter(this->proc,&temp,this->userFilter,this->stateFilter);
    this->n = temp;
    this->proc = filter(this->proc,&(this->n),this->nfilter);
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
    ProcTableModel * model = (ProcTableModel *)this->ui->tableView->model();
    int pid = model->getData(ind.row(),1).toInt();
    kill(pid,SIGKILL);
    this->updateTable();
}

void CubeMainWindow::updateTable() {
    QModelIndex ind = this->ui->tableView->currentIndex();
    ProcTableModel * model = (ProcTableModel *)this->ui->tableView->model();
    int pid = model->getData(ind.row(),1).toInt();

    int t = this->n;
    this->pids = getPids(&n);
    this->proc = updateProcess(this->proc,t,this->pids,this->n);

    this->printTable();

    int nPid = getPidNumber(this->proc, this->n, pid);

    this->ui->tableView->setCurrentIndex(this->ui->tableView->model()->index(nPid,2));


    if (nPid != -1 && this->ui->tableView->hasFocus()) {
        this->ui->tableView->setFocus();
    }
}

void CubeMainWindow::on_pushButton_2_clicked() {
    this->updateTable();
}

void CubeMainWindow::rehreshTable(){
    switch(this->ui->tabWidget->currentIndex()){
    case 0: {

        this->updateTable();
        break;
    }
    case 1: {
        this->ui->processorName->setText(getProcessorName());
        this->ui->frequency->setText(getProcessorFrequency());
        this->ui->cores->setText(QString::number(getProcessorNumberOfCore()));
        int usage = CPU_usage(this->cpu_used);
        this->ui->progressBar->setValue(usage);
        break;
    }
    default:
        break;
    }
}

void CubeMainWindow::sortTable(int coll){
    this->flags[coll] *= -1;
    for (int i = 0; i < 8; i++){
        this->sotrFlag[i] = 0;
    }
    this->sotrFlag[coll] = 1;
    sort(this->proc,this->n, coll, this->flags[coll]);
    this->updateTable();
}

void CubeMainWindow::setMyProcessFilter(bool) {
    this->userFilter = THIS_USER;
    this->updateTable();
}

void CubeMainWindow::setRootProcessFilter(bool) {
    this->userFilter = ROOT;
    this->updateTable();
}

void CubeMainWindow::setAllProcessFilter(bool) {
    this->userFilter = ALL_USERS;
    this->updateTable();
}

void CubeMainWindow::setSleepingStareFilter(bool) {
    this->stateFilter = SLEEPING;
    this->updateTable();
}

void CubeMainWindow::setRunningStateFilter(bool) {
    this->stateFilter = RUNNING;
    this->updateTable();
}

void CubeMainWindow::setAllStateFilter(bool) {
    this->stateFilter = ALL;
    this->updateTable();
}

void CubeMainWindow::on_lineEdit_textChanged(const QString &filter) {
    strcpy(this->nfilter,filter.toStdString().c_str());
    this->updateTable();
}
