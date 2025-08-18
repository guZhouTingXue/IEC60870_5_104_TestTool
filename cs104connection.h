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

public slots:
    void connectToServer();
public:
signals:
    void connectionEvent(int);
private:
    CS104_Connection m_con;
};

#endif // CS104CONNECTION_H
