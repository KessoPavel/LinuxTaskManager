#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "logic.h"
#include <signal.h>
#include <unistd.h>
#include <pthread.h>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->pids = getPids(&n);
    this->proc = getProcess(pids, n);

    this->printTable();

    pthread_create(&this->mainthread,NULL,MainWindow::work,this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::printTable(){
    model = new  ProcTableModel(this->proc, this->n, NULL);
    ui->tableView->setModel(model);

    ui->tableView->verticalHeader()->hide();

    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableView->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
}

/*void MainWindow::printTable(){
    qRegisterMetaType<Qt::Orientation>("Qt::Orientation");
    qRegisterMetaType<QVector<int> >("QVector<int>");
    qRegisterMetaType<QItemSelection>("QItemSelection");
    //ui->tableWidget->clear();
    ui->tableWidget->clearContents();
    ui->tableWidget->verticalHeader()->hide();

    QStringList list;

    ui->tableWidget->setRowCount(n);
    ui->tableWidget->setColumnCount(8);

    list << "Name" << "Pid" << "PPid" << "Priority" << "CPU" << "State" << "Memory" << "Owner";

    ui->tableWidget->setHorizontalHeaderLabels(list);

    for(int i = 0; i < n; i++ ){
        QString * names = new QString(this->proc[i]->name);
        /*QTableWidgetItem * name = new QTableWidgetItem(*names);

        //name->setFlags(Qt::ItemIsEditable);
        name->setTextColor(Qt::black);
        name->setTextAlignment(Qt::AlignCenter);
        ui->tableWidget->setItem(i,0,name);

        QTableWidgetItem * pid = new QTableWidgetItem(QString::number(this->proc[i]->pid));

        //pid->setFlags(Qt::ItemIsEditable);
        pid->setTextColor(Qt::black);
        pid->setTextAlignment(Qt::AlignCenter);
        ui->tableWidget->setItem(i,1,pid);

        QTableWidgetItem * ppid = new QTableWidgetItem(QString::number(this->proc[i]->ppid));

        //ppid->setFlags(Qt::ItemIsEditable);
        ppid->setTextColor(Qt::black);
        ppid->setTextAlignment(Qt::AlignCenter);
        ui->tableWidget->setItem(i,2,ppid);

        QTableWidgetItem * priority = new QTableWidgetItem(*(new QString(getPriority( this->proc[i]->priority))));

        //priority->setFlags(Qt::ItemIsEditable);
        priority->setTextColor(Qt::black);
        priority->setTextAlignment(Qt::AlignCenter);
        ui->tableWidget->setItem(i,3,priority);

        QTableWidgetItem * cpu = new QTableWidgetItem((QString::number(this->proc[i]->cpu) + "%"));

        //cpu->setFlags(Qt::ItemIsEditable);
        cpu->setTextColor(Qt::black);
        cpu->setTextAlignment(Qt::AlignCenter);
        ui->tableWidget->setItem(i,4,cpu);

        QTableWidgetItem * state = new QTableWidgetItem(*(new QString(this->proc[i]->state)));

       // state->setFlags(Qt::ItemIsEditable);
        state->setTextColor(Qt::black);
        state->setTextAlignment(Qt::AlignCenter);
        ui->tableWidget->setItem(i,5,state);

        QTableWidgetItem * memory = new QTableWidgetItem(*(new QString(memoryToString(this->proc[i]->memory))));

        //memory->setFlags(Qt::ItemIsEditable);
        memory->setTextColor(Qt::black);
        memory->setTextAlignment(Qt::AlignCenter);
        ui->tableWidget->setItem(i,6,memory);

        QTableWidgetItem * owner = new QTableWidgetItem(*(new QString(this->proc[i]->owner)));

        //owner->setFlags(Qt::ItemIsEditable);
        owner->setTextColor(Qt::black);
        owner->setTextAlignment(Qt::AlignCenter);
        ui->tableWidget->setItem(i,7,owner);

        //ui->tableWidget->resizeColumnsToContents();

        ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
        ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);

    }
}

void MainWindow::updateTable(){
    qRegisterMetaType<Qt::Orientation>("Qt::Orientation");
    qRegisterMetaType<QVector<int> >("QVector<int>");
    qRegisterMetaType<QItemSelection>("QItemSelection");
    for(int i = 0; i < n; i++){
        QTableWidgetItem * priority = new QTableWidgetItem(*(new QString(getPriority( this->proc[i]->priority))));

        //priority->setFlags(Qt::ItemIsEditable);
        priority->setTextColor(Qt::black);
        priority->setTextAlignment(Qt::AlignCenter);
        ui->tableWidget->setItem(i,3,priority);

        QTableWidgetItem * cpu = new QTableWidgetItem((QString::number(this->proc[i]->cpu) + "%"));

        //cpu->setFlags(Qt::ItemIsEditable);
        cpu->setTextColor(Qt::black);
        cpu->setTextAlignment(Qt::AlignCenter);
        ui->tableWidget->setItem(i,4,cpu);

        QTableWidgetItem * state = new QTableWidgetItem(*(new QString(this->proc[i]->state)));

       // state->setFlags(Qt::ItemIsEditable);
        state->setTextColor(Qt::black);
        state->setTextAlignment(Qt::AlignCenter);
        ui->tableWidget->setItem(i,5,state);

        QTableWidgetItem * memory = new QTableWidgetItem(*(new QString(memoryToString(this->proc[i]->memory))));

        //memory->setFlags(Qt::ItemIsEditable);
        memory->setTextColor(Qt::black);
        memory->setTextAlignment(Qt::AlignCenter);
        ui->tableWidget->setItem(i,6,memory);

        //ui->tableWidget->resizeColumnsToContents();
    }
}
*/
/*void MainWindow::work(){
    int * newPids;
    int newSize;


    while(true){
       usleep(5000000);

       newPids = getPids(&newSize);
       if(equals(this->pids,newPids,this->n,newSize)){
           updateProcess(this->proc,this->n);
       }
       else{
           this->proc = getProcess(this->pids, this->n);
       }
       this->printTable();
    }
}*/

void* MainWindow::work(void* arg){
    MainWindow * th = (MainWindow*)arg;
    while(true){
        emit th->on_pushButton_2_clicked();
      // usleep(1000000);
      //th->pids = getPids(&th->n);
      // th->proc = getProcess(th->pids, th->n);

      // th->printTable();
    }
}


void MainWindow::on_pushButton_clicked()
{
    QModelIndex ind = this->ui->tableView->currentIndex();
    kill(this->pids[ind.row()],SIGKILL);
}

void MainWindow::on_pushButton_2_clicked()
{
    this->pids = getPids(&n);
    this->proc = getProcess(pids, n);

    this->printTable();
}
