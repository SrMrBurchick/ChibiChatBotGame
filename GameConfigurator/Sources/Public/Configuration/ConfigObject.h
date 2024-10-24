#pragma once

#include <QJsonDocument>
#include <QMultiMap>
#include <QObject>
#include <QString>
#include <QColor>
#include <QJsonArray>
#include <QQmlEngine>

#include "Components/ActionComponent.h"
#include "Models/PredefinedActionsModel.h"
#include "Core/Action.h"

class ActionsManager;

class SpriteSize : public QObject {
    Q_OBJECT
    Q_PROPERTY(int height MEMBER Height)
    Q_PROPERTY(int width MEMBER Width)

public:
    int Height = 256;
    int Width = 256;
};

class TableSize : public QObject {

    Q_OBJECT
    Q_PROPERTY(int columns MEMBER Columns)
    Q_PROPERTY(int rows MEMBER Rows)

public:
    int Columns = 13;
    int Rows = 4;
};


class MessageSettings : public QObject {

    Q_OBJECT
    Q_PROPERTY(int fontSize MEMBER FontSize);
    Q_PROPERTY(QColor messageColor MEMBER MessageTextColor)
    Q_PROPERTY(QColor messageBorderColor MEMBER MessageBorderColor)
    Q_PROPERTY(QString font MEMBER Font)

public:

    int FontSize = 24;
    QColor MessageTextColor = "red";
    QColor MessageBorderColor = "black";
    QString Font;
};

class TwitchBotSettings : public QObject {
    Q_OBJECT
    Q_PROPERTY(QString url MEMBER WebSockURL)
    Q_PROPERTY(int port MEMBER WebSockPort)

public:
    QString WebSockURL = "localhost";
    int WebSockPort = 6565;
};


class TwitchSettings : public QObject{
    Q_OBJECT
    Q_PROPERTY(TwitchBotSettings* bot READ getBot)
    Q_PROPERTY(QString channelName MEMBER ChannelName)

public:
    TwitchBotSettings Bot;

    TwitchBotSettings* getBot(){
        TwitchBotSettings* OutBot = &Bot;
        QQmlEngine::setObjectOwnership(OutBot, QQmlEngine::CppOwnership);
        return OutBot;
    }

    QString OAuthToken = "";
    QString ChannelName = "";
    QString UserId = "";
};

class GameSettings : public QObject {

    Q_OBJECT
    Q_PROPERTY(float spriteScale MEMBER SpriteScale)
    Q_PROPERTY(int screenHeight MEMBER ScreenHeight)
    Q_PROPERTY(int screenWidth MEMBER ScreenWidth)
    Q_PROPERTY(float actionExecutionTime MEMBER ActionExecutionTime)
    Q_PROPERTY(float movementSpeed MEMBER MovementSpeed)
    Q_PROPERTY(float nextActionTimeout MEMBER NextActionTimeout)

public:
    float SpriteScale = 1.0f;
    int ScreenHeight = 1080;
    int ScreenWidth = 1920;
    float ActionExecutionTime = 10.0f;
    float MovementSpeed = 3500.0f;
    float NextActionTimeout = 20.0f;
};

class SystemConfig : public QObject {

    Q_OBJECT
    Q_PROPERTY(QString imagePath MEMBER ImagePath)
    Q_PROPERTY(bool logger MEMBER Logging)
    Q_PROPERTY(TwitchSettings* twitchSettings READ getTwitchSettings)
    Q_PROPERTY(MessageSettings* messageSettings READ getMessageSettings)
    Q_PROPERTY(GameSettings* gameSettings READ getGameSettings)

public:
    QString ImagePath = "qrc:/qml/Images/sprite-sheet.png";
    bool Logging = false;
    TwitchSettings Twitch;
    MessageSettings Message;
    GameSettings Game;

    TwitchSettings* getTwitchSettings() {
        TwitchSettings* OutTwitchSettings = &Twitch;
        QQmlEngine::setObjectOwnership(OutTwitchSettings, QQmlEngine::CppOwnership);
        return OutTwitchSettings;
    }

    MessageSettings* getMessageSettings() {
        MessageSettings* OutMessageSettings = &Message;
        QQmlEngine::setObjectOwnership(OutMessageSettings, QQmlEngine::CppOwnership);
        return OutMessageSettings;
    }

    GameSettings* getGameSettings() {
        GameSettings* OutGameSettings = &Game;
        QQmlEngine::setObjectOwnership(OutGameSettings, QQmlEngine::CppOwnership);
        return OutGameSettings;
    }
};

class ConfigObject : public QObject {
    Q_OBJECT

    Q_PROPERTY(bool isBusy MEMBER bIsBusy NOTIFY busyUpdated)
    Q_PROPERTY(bool isLoaded MEMBER bConfigLoaded NOTIFY configured)
    Q_PROPERTY(SpriteSize* spriteSize READ getSpriteSize)
    Q_PROPERTY(TableSize* tableSize READ getTableSize)
    Q_PROPERTY(SystemConfig* systemConfig READ getSystemConfig)
    Q_PROPERTY(PredefinedActionsListModel* predefinedActionsModel READ getPredefinedActionsModel)

public:
    explicit ConfigObject(QObject* Parent = nullptr);
    virtual ~ConfigObject();

    //============================ C++ ========================================
    // Modifiers
    void ParseJsonDocument(const QJsonDocument& JsonDocument);
    void SaveConfigToFile(const QString& ConfigFile);
    void InitActionsManager(ActionsManager* Manager) const;

    void SaveLogging(bool Logging);
    void SaveActions(const QVector<QSharedPointer<Action>>& Actions);

    // Getters
    SpriteSize* getSpriteSize() {
        SpriteSize* OutSpriteSize = &SpriteSettings;
        QQmlEngine::setObjectOwnership(OutSpriteSize, QQmlEngine::CppOwnership);
        return OutSpriteSize;
    }

    TableSize* getTableSize() {
        TableSize* OutTableSize = &TableSettings;
        QQmlEngine::setObjectOwnership(OutTableSize, QQmlEngine::CppOwnership);
        return OutTableSize;
    }

    SystemConfig* getSystemConfig() {
        SystemConfig* OutSystemConfig = &SystemSettings;
        QQmlEngine::setObjectOwnership(OutSystemConfig, QQmlEngine::CppOwnership);
        return OutSystemConfig;
    }

    PredefinedActionsListModel* getPredefinedActionsModel() {
        PredefinedActionsListModel* OutModel = PredefinedActionsModel.get();
        QQmlEngine::setObjectOwnership(OutModel, QQmlEngine::CppOwnership);
        return OutModel;
    }

    //============================ QML ========================================
    // Modifiers
    Q_INVOKABLE void saveConfig();
    Q_INVOKABLE void loadConfig();
    Q_INVOKABLE void saveTwitchInfo(const QString& ChannelName, const QString& OAuthToken, const QString& UserId);
    Q_INVOKABLE void setLoggerEnabled(bool Enabled);
    Q_INVOKABLE void saveDataToClipboard(const QString& Data);

signals:
    void actionsConfigured();
    void busyUpdated();
    void configured();
    void loggerEnabled(bool Enabled);

protected:
    void CopyImageToAssets();
    void CopyFontToAssets();
    void SetBusy(bool isBusy);

    bool bConfigLoaded = false;
    SpriteSize SpriteSettings;
    TableSize TableSettings;
    SystemConfig SystemSettings;
    QPointer<PredefinedActionsListModel> PredefinedActionsModel;

    // List of the animations
    ActionsMap Map;

    QJsonArray ActionsArray;

    bool bIsBusy = false;
};
