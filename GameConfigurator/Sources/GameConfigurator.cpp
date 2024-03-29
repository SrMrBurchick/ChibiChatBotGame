#include <iostream>

#include <QGuiApplication>
#include <QQmlApplicationEngine>

#include "Configuration/ConfigObject.h"
#include "Managers/NotificationsManager.h"
#include "Managers/ProcessManager.h"
#include "Managers/Processes/ChatBotProcess.h"
#include "Managers/Processes/GameProcess.h"
#include "Models/ActionsListModel.h"
#include "Models/AnimationSequenceModel.h"
#include "Models/NotificationModel.h"
#include "Models/PredefinedActionsModel.h"
#include "Models/SpriteSheet.h"
#include "System/Logger.h"

int main(int argc, char *argv[])
{
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif

    QGuiApplication app(argc, argv);
    QQmlApplicationEngine engine;

    QScopedPointer<ConfigObject> Config(new ConfigObject);
    QScopedPointer<ProcessManager> Manager(new ProcessManager);
    QPointer<IProcess> Game(new GameProcess);
    QPointer<IProcess> ChatBot(new ChatBotProcess);

    Manager->AddProcess(Game);
    Manager->AddProcess(ChatBot);

    engine.addImportPath(":/qml");

    // Setup config signals
    QObject::connect(Config.get(), &ConfigObject::loggerEnabled, [&](bool Enabled) {
        Logger::SetLoggerEnabled(Enabled);
        Config->SaveLogging(Enabled);
    });

    ActionsListModel::registerModel("GameActions");
    AnimationSequenceModel::registerModel("GameActions");
    SpriteSheetModel::registerModel("GameActions");
    NotificationListModel::registerModel("SystemTools");
    PredefinedActionsListModel::registerModel("ConfigTools");

    qmlRegisterSingletonInstance("ConfigComponent", 1, 0, "Config", Config.get());
    qmlRegisterSingletonInstance("ProcessesComponent", 1, 0, "ProcessManager", Manager.get());
    qmlRegisterSingletonInstance("SystemTools", 1, 0, "NotificationsManager", NotificationsManager::GetManager().get());

    const QUrl url(QStringLiteral("qrc:/main_window.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
                     if (!obj && url == objUrl)
                     QCoreApplication::exit(-1);
                     }, Qt::QueuedConnection);
    engine.load(url);

    return app.exec();
}
