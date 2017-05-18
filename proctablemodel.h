#ifndef PROCTABLEMODEL_H
#define PROCTABLEMODEL_H

#include <QAbstractTableModel>
#include "process.h"
#include <QList>
#include <QVariant>


class ProcTableModel : public QAbstractTableModel
{
    //Q_OBJECT

private:
    QList<process*> procList;
public:
    ProcTableModel();
    ProcTableModel(process**, int, QObject * parent);

private:
    int rowCount(const QModelIndex &parent) const;
    int columnCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;
    Qt::ItemFlags flags(const QModelIndex &index) const;
    bool setData(const QModelIndex &index, const QVariant &value, int role);
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;

public:
    QVariant getData(int, int);

};

#endif // PROCTABLEMODEL_H
