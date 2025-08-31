#include "cs101apdu.h"


APDU::APDU(APCI *apci, ASDU *asdu, const QDateTime &time, QObject *parent)
    : m_apci(apci)
    , m_asdu(asdu)
    , m_time(time)
{

}

QString APCI::display() {
    return QString();
}
