#include <QGuiApplication>
#include <QQmlApplicationEngine>

//#include "cs104connection.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;
    //qmlRegisterType<CS104Connection>("ming.connection", 1, 0, "CS104Connection");

    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreationFailed,
        &app,
        []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);
    engine.loadFromModule("IEC60870_5_104_TestTool", "Main");

    return app.exec();
}
