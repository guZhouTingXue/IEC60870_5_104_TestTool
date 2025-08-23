#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QThread>
#include "cs104connection.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;
    //qmlRegisterType<CS104Connection>("ming.connection", 1, 0, "CS104Connection");
    CS104Connection *cs104 = new CS104Connection(&app);
    engine.setInitialProperties({{ "cs104", QVariant::fromValue(cs104) }});
    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreationFailed,
        &app,
        []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);
    engine.loadFromModule("IEC60870_5_104_TestTool", "Main");

    return app.exec();
}
