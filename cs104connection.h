#ifndef CS104CONNECTION_H
#define CS104CONNECTION_H

#include <QObject>
#include <QQmlEngine>

#include "cs104_connection.h"
#include "hal_time.h"
#include "hal_thread.h"

class CS104Connection : public QObject
{
    Q_OBJECT
    QML_ELEMENT
public:
    explicit CS104Connection(QObject *parent = nullptr);
    ~CS104Connection();
public slots:
    void connectToServer();
    void disconnectToServer();

    void sendStartDT();

public:
    void setConnectionEvent(CS104_ConnectionEvent event);
    CS104_Connection getConnection() const { return m_con; }
signals:
    void connectionEvent(int);

private:
    CS104_Connection m_con;
    CS104_ConnectionEvent m_cntEvent = CS104_CONNECTION_CLOSED;
};

#endif // CS104CONNECTION_H
