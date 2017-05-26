#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "logic.h"
#include <signal.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <QLayoutItem>
#include "processorlogic.h"
#include "rehreshthread.h"
#include "raminfo.h"

CubeMainWindow::CubeMainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow) {

    ui->setupUi(this);

    this->setWindowTitle("Cube");
    init();
    initMem();
    processorInfinit();

    this->cpux = new QLabel *[getProcessorNumberOfCore()];
    this->cpuxP = new QProgressBar *[getProcessorNumberOfCore()];
    for(int i = 0; i < getProcessorNumberOfCore(); i++ ){
	char c[20];
        sprintf(c,"cpu%d :", i);
        this->cpux[i] = new QLabel(c);
        this->cpux[i]->setMinimumWidth(40);
        this->cpuxP[i] = new QProgressBar();
    }

    for(int i = 0; i < getProcessorNumberOfCore(); i++){
        this->ui->cpux->addWidget(this->cpux[i]);
        this->ui->cpux->addWidget(this->cpuxP[i]);
    }

    this->cpu_used = (CPU*)malloc(sizeof(CPU)*1);
    cpuInit(this->cpu_used);

    for(int i = 0; i < 8; i++){
        this->flags[i] = 1;
        this->sotrFlag[i] = 0;
    }
    this->userFilter = ALL_USERS;
    this->stateFilter = ALL;
    this->nfilter  = (char*)calloc(150,sizeof(char));

    this->pids = getPids(&n);
    this->proc = getProcess(pids, n);


    this->printTable();

    this->rehreshThread = new RehreshThread(this);

    connect(this->rehreshThread,SIGNAL(rehresh()),this,SLOT(rehreshTable()));
    connect(this->ui->tableView->horizontalHeader(),SIGNAL(sectionClicked(int)),this,SLOT(sortTable(int)));

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
    this->proc = setFilter(this->proc,&this->n,this->userFilter,this->stateFilter);
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
        int* usage = CPU_usage(this->cpu_used);
        this->ui->progressBar->setValue(usage[0]);

        for(int i = 0; i < getProcessorNumberOfCore(); i++){
            this->cpuxP[i]->setValue(usage[i + 1]);
        }

        int totalRam = getMemTotal();

        int gb = (totalRam / 1024) / 1024;
        int mb = (totalRam-gb*1024*1024) /1024;
        int kb = (totalRam - (gb*1024*1024+mb*1024));

	char Ram[100];
        sprintf(Ram,"%d Gb %d Mb %d Kb",gb,mb,kb);

        this->ui->TotalRAM->setText(Ram);

        int freeRam = getMemFree();
        gb = (freeRam / 1024) / 1024;
        mb = (freeRam-gb*1024*1024) /1024;
        kb = (freeRam - (gb*1024*1024+mb*1024));

        sprintf(Ram,"%d Gb %d Mb %d Kb",gb,mb,kb);

        this->ui->FreeRAM->setText(Ram);

        this->ui->progressBar_2->setValue((totalRam - freeRam)*100 / totalRam);

        free(usage);
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
