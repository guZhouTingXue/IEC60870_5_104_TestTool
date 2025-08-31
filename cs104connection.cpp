#include "cs104connection.h"

#include <QDebug>

static void
rawMessageHandler (void* parameter, uint8_t* msg, int msgSize, bool sent)
{
    qDebug() << "msgSize:" << msgSize;
    if (sent)
        qDebug("SEND: ");
    else
        qDebug("RCVD: ");

    CS104Connection *con = static_cast<CS104Connection*>(parameter);
    if(!con)
    {
        qDebug() << __func__ << "error: static_cast<CS104Connection*>(parameter)";
        return ;
    }

    bool isSFormatFrame= false; //是否为s帧  S format frame
    if( (msgSize == 6)
        && (msg[0] == 0x68)
        && (msg[1] == 0x04)
        && (msg[2] == 0x01) )
    {
        APCI *apci = new APCI_S(msg);
        APDU *apdu = new APDU(apci);
        apdu->setSent(sent);
        con->appendAPDU(apdu);
        emit con->recvAPDU(apdu);
    }
    else if(  (msgSize == 6)
               && (msg[0] == 0x68)
               && (msg[1] == 0x04)
               && ( (msg[2] & 0x03) == 0x03))
    {
        APCI *apci = new APCI_U(msg);
        APDU *apdu = new APDU(apci);
        apdu->setSent(sent);
        con->appendAPDU(apdu);
        emit con->recvAPDU(apdu);
    }

    // QString msgStr;
    // int i;
    // for (i = 0; i < msgSize; i++) {
    //     msgStr += " " + QString::number(msg[i], 16);
    // }

    //qDebug() << msgStr;
}


static bool
asduReceivedHandler (void* parameter, int address, CS101_ASDU asdu)
{
    qDebug("RECVD ASDU type: %s(%i) elements: %i\n",
           TypeID_toString(CS101_ASDU_getTypeID(asdu)),
           CS101_ASDU_getTypeID(asdu),
           CS101_ASDU_getNumberOfElements(asdu));

    CS104Connection *con = static_cast<CS104Connection*>(parameter);
    if(!con)
    {
        qDebug() << __func__ << "error: static_cast<CS104Connection*>(parameter)";
        return true;
    }

    auto applayerParameters= CS104_Connection_getAppLayerParameters(con->getConnection());
    int asduHeaderLength = 2 + applayerParameters->sizeOfCOT + applayerParameters->sizeOfCA;
    auto msg = CS101_ASDU_getPayload(asdu);
    // qDebug() << "msg:";
    // QString msgStr;
    // for (int j = 0; j < 10; j++) {
    //     msgStr += " " + QString::number(*(msg - asduHeaderLength - 6 + j), 16).rightJustified(2, '0');
    // }
    // qDebug() << msgStr;
    auto buffer = msg - asduHeaderLength - 6;

    APCI *apci = new APCI_I(buffer);
    auto sq = CS101_ASDU_isSequence(asdu);
    auto num = CS101_ASDU_getNumberOfElements(asdu);
    ASDU::VSQ vsq(sq, num);
    auto test = CS101_ASDU_isTest(asdu);
    auto negative = CS101_ASDU_isNegative(asdu);
    auto cot = CS101_ASDU_getCOT(asdu);
    ASDU::COT m_cot(test, negative, cot);
    auto typeID = CS101_ASDU_getTypeID(asdu);
    auto ca = CS101_ASDU_getCA(asdu);
    ASDU *m_asdu = new ASDU(typeID, vsq, m_cot);
    m_asdu->setCA(ca);

    if (CS101_ASDU_getTypeID(asdu) == M_ME_NC_1) {
        int i;
        auto number = CS101_ASDU_getNumberOfElements(asdu);
        qDebug(" measured short values n:%d\n", number);

        for (i = 0; i < number; i++) {

            MeasuredValueShort io =
                (MeasuredValueShort) CS101_ASDU_getElement(asdu, i);

            auto ioa = InformationObject_getObjectAddress((InformationObject) io);
            auto value = MeasuredValueShort_getValue((MeasuredValueShort) io);
            qDebug("    IOA: %i value: %f\n", ioa, value );
            InformationElement *ele = new InformationElement(ioa, value);
            m_asdu->addInformationElement(ele);
            MeasuredValueShort_destroy(io);
        }
    }
    else if(CS101_ASDU_getTypeID(asdu) == M_ME_NB_1)
    {
        int i;
        auto number = CS101_ASDU_getNumberOfElements(asdu);
        qDebug("scaled value  n:%d\n", number);

        for (i = 0; i < number; i++) {

            MeasuredValueScaled io =
                (MeasuredValueScaled) CS101_ASDU_getElement(asdu, i);

            auto ioa = InformationObject_getObjectAddress((InformationObject) io);
            auto value = MeasuredValueScaled_getValue((MeasuredValueScaled) io);
            qDebug("    IOA: %i value: %d\n", ioa, value );
            InformationElement *ele = new InformationElement(ioa, value);
            m_asdu->addInformationElement(ele);
            MeasuredValueScaled_destroy(io);
        }
    }
    APDU * apdu = new APDU(apci, m_asdu);
    con->appendAPDU(apdu);
    emit con->recvAPDU(apdu);

    return true;
}


/* Connection event handler */
static void
connectionHandler (void* parameter, CS104_Connection connection, CS104_ConnectionEvent event)
{
    CS104Connection *con = static_cast<CS104Connection*>(parameter);
    if(con)
    {
        con->setConnectionEvent(event);
        emit con->connectionEvent(event);
    }
    switch (event) {
    case CS104_CONNECTION_OPENED:
        qDebug("Connection established\n");
        break;
    case CS104_CONNECTION_CLOSED:
        qDebug("Connection closed\n");
        break;
    case CS104_CONNECTION_STARTDT_CON_RECEIVED:
        qDebug("Received STARTDT_CON\n");
        break;
    case CS104_CONNECTION_STOPDT_CON_RECEIVED:
        qDebug("Received STOPDT_CON\n");
        break;
    case CS104_CONNECTION_FAILED:
        qDebug("Connection failed\n");
        break;
    }
}

CS104Connection::CS104Connection(QObject *parent)
    : QObject{parent}
{
    qDebug() << __FUNCTION__ ;
    const char* ip = "localhost";
    uint16_t port = IEC_60870_5_104_DEFAULT_PORT;
    const char* localIp = NULL;
    int localPort = -1;

    qDebug("Connecting to: %s:%i\n", ip, port);
    m_con = CS104_Connection_create(ip, port);

    CS101_AppLayerParameters alParams = CS104_Connection_getAppLayerParameters(m_con);
    alParams->originatorAddress = 3;

    CS104_Connection_setConnectionHandler(m_con, connectionHandler, this);
    CS104_Connection_setASDUReceivedHandler(m_con, asduReceivedHandler, this);
    CS104_Connection_setRawMessageHandler(m_con, rawMessageHandler, this);

}

CS104Connection::~CS104Connection()
{
     if(m_cntEvent != CS104_CONNECTION_FAILED || m_cntEvent != CS104_CONNECTION_CLOSED)
     {
         CS104_Connection_close(m_con);
         CS104_Connection_destroy(m_con);
     }
}

void CS104Connection::connectToServer()
{
    CS104_Connection_connectAsync(m_con);
}

void CS104Connection::disconnectToServer()
{
    CS104_Connection_close(m_con);
}

void CS104Connection::sendStartDT()
{
    CS104_Connection_sendStartDT(m_con);
}

void CS104Connection::setConnectionEvent(CS104_ConnectionEvent event)
{
    m_cntEvent = event;
    if(event == CS104_CONNECTION_OPENED)
    {
        sendStartDT();
    }
}
