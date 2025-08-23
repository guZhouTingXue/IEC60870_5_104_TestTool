#include "cs104connection.h"

#include <QDebug>

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
