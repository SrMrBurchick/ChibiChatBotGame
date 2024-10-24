#include <iostream>

#include <QGuiApplication>
#include <QQmlApplicationEngine>

#include "Configuration/ConfigObject.h"
#include "Managers/NotificationsManager.h"
#include "Managers/ProcessManager.h"
#include "Managers/Processes/BotProcess.h"
#include "Managers/Processes/GameProcess.h"
#include "Models/ActionsListModel.h"
#include "Models/AnimationSequenceModel.h"
#include "Models/NotificationModel.h"
#include "Models/PredefinedActionsModel.h"
#include "Models/SpriteSheet.h"
#include "Managers/ActionsManager.h"
#include "Managers/TwitchManager.h"
#include "Core/Action.h"
#include "System/Logger.h"
#include "System/FontSelector.h"
#include "Managers/TwitchNetworkAccessManager.h"
#include "Core/Twitch/ChannelPointsReward.h"
#include "Models/Twitch/ChannelPointsRewardsModel.h"

int main(int argc, char *argv[])
{
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif

    QGuiApplication app(argc, argv);
    QQmlApplicationEngine engine;

    Logger::SetLoggerEnabled(true);

    QScopedPointer<ConfigObject> Config(new ConfigObject);
    QScopedPointer<ProcessManager> Manager(new ProcessManager);
    QScopedPointer<ActionsManager> ActionsManagerComp(new ActionsManager);
    QScopedPointer<TwitchManager> Twitch(new TwitchManager);
    QScopedPointer<CBFontSelector> FontSelector(new CBFontSelector);
    QPointer<TwitchNetworkAccessManager> TwitchNtwrk (new TwitchNetworkAccessManager);
    QPointer<IProcess> Game(new GameProcess);
    QPointer<IProcess> Bot(new BotProcess);

    Twitch->SetNetworkManager(TwitchNtwrk);
    Manager->AddProcess(Game);
    Manager->AddProcess(Bot);

    engine.addImportPath(":/qml");

    Logger::SetLoggerEnabled(true);
    // Setup config signals
    QObject::connect(Config.get(), &ConfigObject::loggerEnabled, [&](bool Enabled) {
        // Logger::SetLoggerEnabled(Enabled);
        Config->SaveLogging(Enabled);
    });

    // Models
    ActionsListModel::registerModel("ActionsModels");
    AnimationSequenceModel::registerModel("ActionsModels");
    SpriteSheetModel::registerModel("ActionsModels");
    NotificationListModel::registerModel("SystemTools");
    PredefinedActionsListModel::registerModel("ConfigTools");
    ChannelPointsRewardsModel::registerModel("TwitchModels");

    // Instances
    qmlRegisterSingletonInstance("ConfigComponent", 1, 0, "Config", Config.get());
    qmlRegisterSingletonInstance("Managers", 1, 0, "ProcessManager", Manager.get());
    qmlRegisterSingletonInstance("Managers", 1, 0, "ActionsManager", ActionsManagerComp.get());
    qmlRegisterSingletonInstance("Managers", 1, 0, "NotificationsManager", NotificationsManager::GetManager().get());
    qmlRegisterSingletonInstance("Managers", 1, 0, "TwitchManager", Twitch.get());
    qmlRegisterSingletonInstance("Managers", 1, 0, "FontSelector", FontSelector.get());

    // Types
    qmlRegisterType<Action>("ActionsManagerComponent", 1, 0, "Action");
    qmlRegisterType<ActionConfig>("ActionsManagerComponent", 1, 0, "ActionConfig");
    qmlRegisterType<ChannelPointsReward>("TwitchManagerComponent", 1, 0, "ChannelPointsReward");
    qmlRegisterType<SpriteSize>("ConfigComponent", 1, 0, "SpriteSize");
    qmlRegisterType<TwitchBotSettings>("ConfigComponent", 1, 0, "TwitchBotSettings");
    qmlRegisterType<TwitchSettings>("ConfigComponent", 1, 0, "TwitchSettings");
    qmlRegisterType<GameSettings>("ConfigComponent", 1, 0, "GameSettings");
    qmlRegisterType<MessageSettings>("ConfigComponent", 1, 0, "MessageSettings");
    qmlRegisterType<GameSettings>("ConfigComponent", 1, 0, "GameSettings");

    const QUrl url(QStringLiteral("qrc:/main_window.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
                     if (!obj && url == objUrl)
                     QCoreApplication::exit(-1);
                     }, Qt::QueuedConnection);
    engine.load(url);

    return app.exec();
}
