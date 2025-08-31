#ifndef APDUMODEL_H
#define APDUMODEL_H

#include <QAbstractTableModel>
#include <qqml.h>

#include "cs101apdu.h"

class APDUModel : public QAbstractTableModel
{
    Q_OBJECT
    QML_ELEMENT
public:
    explicit APDUModel(QObject *parent = nullptr);

    // Header:
    QVariant headerData(int section,
                        Qt::Orientation orientation,
                        int role = Qt::DisplayRole) const override;

    // Basic functionality:
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    // QHash<int, QByteArray> roleNames() const override {
    //     return { {Qt::DisplayRole, "display"}};
    // }

public slots:
    void recvAPDU(APDU *apdu);
private:
    QList<APDU *> m_apduList;
    QList<size_t> m_beginRow; //记录apud的起始行

};

#endif // APDUMODEL_H
