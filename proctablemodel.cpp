#include "proctablemodel.h"
#include "logic.h"
#include <QColor>
#include <QBrush>

ProcTableModel::ProcTableModel() {

}

ProcTableModel::ProcTableModel(process ** proc, int n,QObject * parent): QAbstractTableModel(parent) {
    this->procList = QList<process*>();
    for(int i = 0; i < n; i++){
        this->procList.push_back(proc[i]);
    }
}

int ProcTableModel::rowCount(const QModelIndex &parent) const {
    Q_UNUSED(parent)
    return procList.size();
}

int ProcTableModel::columnCount(const QModelIndex &parent) const {
    Q_UNUSED(parent)
    return 8;
}

QVariant ProcTableModel::data(const QModelIndex &index, int role) const {
    QString unswer;
    if(role == Qt::DisplayRole){
        switch (index.column()) {
        case 0:
            unswer = QString(this->procList.at(index.row())->name);
            break;
        case 1:
            unswer = QString::number(this->procList.at(index.row())->pid);
            break;
        case 2:
            unswer = QString::number(this->procList.at(index.row())->ppid);
            break;
        case 3:
            unswer = QString(getPriority(this->procList.at(index.row())->priority));
            break;
        case 4:
            unswer = QString::number((this->procList.at(index.row())->cpu)) + "%";
            break;
        case 5:
            unswer = QString(this->procList.at(index.row())->state);
            break;
        case 6:
            unswer = QString(memoryToString(this->procList.at(index.row())->memory));
            break;
        case 7:
            unswer = QString(this->procList.at(index.row())->owner);
            break;
        default:
            break;
        }
        return QVariant(unswer);
    }
    if (role == Qt::BackgroundColorRole) {
        QColor * color = new QColor("white");
        if(index.row() % 2 == 0){
            color->setNamedColor("whitesmoke");
        }
        return QBrush(*color);
    }
    if( role == Qt::TextAlignmentRole){
        if(index.column() == 0 || index.column() == 1 || index.column() == 2 )
	    return unswer;
        return Qt::AlignCenter;
    }
    return QVariant();
}

QVariant ProcTableModel::getData(int row, int col)
{
    QString unswer = QString::number(-1);

    if(row != -1){
        switch (col) {
        case 0:
            unswer = QString(this->procList.at(row)->name);
            break;
        case 1:
            unswer = QString::number(this->procList.at(row)->pid);
            break;
        case 2:
            unswer = QString::number(this->procList.at(row)->ppid);
            break;
        case 3:
            unswer = QString(getPriority(this->procList.at(row)->priority));
            break;
        case 4:
            unswer = QString::number((this->procList.at(row)->cpu)) + "%";
            break;
        case 5:
            unswer = QString(this->procList.at(row)->state);
            break;
        case 6:
            unswer = QString(memoryToString(this->procList.at(row)->memory));
            break;
        case 7:
            unswer = QString(this->procList.at(row)->owner);
            break;
        default:
            break;
        }
        return QVariant(unswer);
    }
    return QVariant(unswer);
}

Qt::ItemFlags ProcTableModel::flags(const QModelIndex &index) const {
    if(!index.isValid())
        return Qt::ItemIsEnabled;
    return QAbstractItemModel::flags(index) | Qt::ItemIsEditable;
}

bool ProcTableModel::setData(const QModelIndex &index, const QVariant &value, int role) {
    if(index.isValid() && role == Qt::EditRole){
        switch (index.column()) {
        case 3:
            this->procList.at(index.row())->priority = atoi(value.toString().toStdString().c_str());
            break;
        case 4:
            this->procList.at(index.row())->cpu = atoi(value.toString().toStdString().c_str());
            break;
        case 5 :
	    strcpy(this->procList.at(index.row())->state,value.toString().toStdString().c_str());
            break;
        case 6:
            this->procList.at(index.row())->memory = atoi(value.toString().toStdString().c_str());
            break;
        default:
            break;
        }

        emit dataChanged(index,index);
        return true;
    }
    return false;
}

QVariant ProcTableModel::headerData(int section, Qt::Orientation orientation, int role) const {

    if (role != Qt::DisplayRole)
        return QVariant();
    if (orientation == Qt::Horizontal)
    {
        switch (section)
        {
        case 0:
            return QVariant(QString::fromUtf8("Name"));
        case 1:
            return QVariant(QString::fromUtf8("Pid"));
        case 2:
            return QVariant(QString::fromUtf8("PPid"));
        case 3:
            return QVariant(QString::fromUtf8("Priority"));
        case 4:
            return QVariant(QString::fromUtf8("CPU"));
        case 5:
            return QVariant(QString::fromUtf8("State"));
        case 6:
            return QVariant(QString::fromUtf8("Memory"));
        case 7:
            return QVariant(QString::fromUtf8("Owner"));
        default:
            return QVariant();
        }
    }
    return QVariant();
}

