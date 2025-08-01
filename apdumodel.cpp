#include "apdumodel.h"
//#include <QDebug>

APDUModel::APDUModel(QObject *parent)
    : QAbstractTableModel(parent)
{}

QVariant APDUModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    // FIXME: Implement me!
    QStringList horizontalHeader;
    horizontalHeader << "#ID" << "Time(received/sent)" << "APCI/LPCI" << "TypeID"
                     << "COT" << "OA" << "CA" << "IOA" << "Value" << "Timestamp" << "Quality";
    if(orientation == Qt::Horizontal && role == Qt::DisplayRole)
    {
        if(section <= horizontalHeader.size() && section >= 0)
            return horizontalHeader.at(section);
    }
    return QVariant();
}

int APDUModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;

    // FIXME: Implement me!
    return 1;
}

int APDUModel::columnCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;
    // FIXME: Implement me!
    return 11;
}

QVariant APDUModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    // FIXME: Implement me!
    if(index.isValid() && role == Qt::DisplayRole)
    {
        return QString("1");
    }

    return QVariant();
}
