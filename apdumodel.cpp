#include "apdumodel.h"
#include <QDebug>

APDUModel::APDUModel(QObject *parent)
    : QAbstractTableModel(parent)
{}

QVariant APDUModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    // FIXME: Implement me!
    QStringList horizontalHeader;
    horizontalHeader << "#ID" << "Time(received/sent)" << "APCI/LPCI" << "TypeID" << "VSQ"
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
    size_t s = 0;
    for(auto apdu : m_apduList)
    {
        auto apci = apdu->apci();
        if(apci->type() == APCI::IFormat)
        {
            auto asdu = apdu->asdu();
            s += asdu->elements().size();
        }
        else
            s++;
    }
    return s;
}

int APDUModel::columnCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;
    // FIXME: Implement me!
    return 12;
}

QVariant APDUModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    auto row = index.row();
    auto column = index.column();

    // FIXME: Implement me!
    if(index.isValid() && role == Qt::DisplayRole)
    {
        QString s;
        size_t br = 0;
        size_t idx = 0;
        for(size_t i = 0; i < m_beginRow.size(); i++)
        {
            auto beginRow = m_beginRow.at(i);
            if(row >= beginRow)
            {
                idx = i;
                br = beginRow;
            }
        }
        APDU *apdu = m_apduList.at(idx);
        APCI *apci = apdu->apci();
        ASDU *asdu = apdu->asdu();
        InformationElement *ele = nullptr;
        if(apci->type() == APCI::IFormat)
            ele = asdu->elements().at(row - br);

        bool firstElement = asdu && (row == br);

        switch(index.column())
        {
        case 0:
            return index.row() + 1;
            break;
        case 1:
            return apdu->time().toString("yyyy.MM.dd hh:mm:ss.zzz");
            break;
        case 2:
            //U,S 短帧报文 和 I中的第一个信息元素显示 类型
            if(!asdu || (row == br))
            {
                if(apdu->sent())
                    return ">>> " + apci->display();
                else
                    return "<<< " + apci->display();
            }
            break;
        case 3:
            if(firstElement)
            {
                if(row == 0 || row == 1)
                    qDebug() << "debug" ;
                return QString(TypeID_toString(asdu->typeId()))
                       + QString(" (%0)").arg((int)asdu->typeId());

            }
            break;
        case 4:
            if(firstElement)
                return QString("SQ:%0, NumIx:%1").arg(asdu->vsq().sequence).arg(asdu->vsq().number);
            break;
        case 5:
            if(firstElement)
                return QString(CS101_CauseOfTransmission_toString(asdu->cot().cot))
                       + QString("(%0)").arg(asdu->cot().cot);
            break;
        case 6:
            if(firstElement)
                return asdu->cot().oa;
            break;
        case 7:
            if(firstElement)
                return asdu->ca();
            break;
        case 8:
            if(ele)
                return ele->ioa();
            break;
        case 9:
            if(ele)
                return ele->value();
            break;
        default:
            return " ";
        }
        return s;
    }

    return QVariant();
}

void APDUModel::recvAPDU(APDU *apdu)
{
    auto row = rowCount();
    beginInsertRows(QModelIndex(), row, row);
    m_beginRow.append(row);
    m_apduList.append(apdu);
    endInsertRows();
}
