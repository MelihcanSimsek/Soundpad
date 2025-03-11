#include <QGuiApplication>
#include <QIcon>
#include <QQmlApplicationEngine>
#include "SoundpadController.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);
    QQmlApplicationEngine engine;

    app.setWindowIcon(QIcon(":/assets/icons/app.ico"));

    SoundpadController *soundController = new SoundpadController(&app);
    qmlRegisterSingletonInstance("com.company.SoundpadController", 1, 0, "SoundpadController", soundController);

    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreationFailed,
        &app,
        []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);
    engine.loadFromModule("CustomSoundpad", "Main");

    return app.exec();
}
