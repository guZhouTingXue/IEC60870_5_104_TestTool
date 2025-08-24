#include "cs104connection.h"

#include <QDebug>

static bool
asduReceivedHandler (void* parameter, int address, CS101_ASDU asdu)
{
    qDebug("RECVD ASDU type: %s(%i) elements: %i\n",
           TypeID_toString(CS101_ASDU_getTypeID(asdu)),
           CS101_ASDU_getTypeID(asdu),
           CS101_ASDU_getNumberOfElements(asdu));

    qDebug() << "asdu payload:" << *CS101_ASDU_getPayload(asdu);

    CS104Connection *con = static_cast<CS104Connection*>(parameter);
    if(!con)
    {
        qDebug() << __func__ << "error: static_cast<CS104Connection*>(parameter)";
        return true;
    }

    auto applayerParameters= CS104_Connection_getAppLayerParameters(con->getConnection());
    int asduHeaderLength = 2 + applayerParameters->sizeOfCOT + applayerParameters->sizeOfCA;
    auto msg = CS101_ASDU_getPayload(asdu);
    qDebug() << "msg:";
    QString msgStr;
    for (int j = 0; j < 10; j++) {
        msgStr += " " + QString::number(*(msg - asduHeaderLength - 6 + j), 16).rightJustified(2, '0');
    }
    qDebug() << msgStr;

    if (CS101_ASDU_getTypeID(asdu) == M_ME_NC_1) {

        qDebug(" measured short values \n");

        int i;

        for (i = 0; i < CS101_ASDU_getNumberOfElements(asdu); i++) {

            MeasuredValueShort io =
                (MeasuredValueShort) CS101_ASDU_getElement(asdu, i);

            qDebug("    IOA: %i value: %f\n",
                   InformationObject_getObjectAddress((InformationObject) io),
                   MeasuredValueShort_getValue((MeasuredValueShort) io)
                   );

            MeasuredValueShort_destroy(io);
        }
    }


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
