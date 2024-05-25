#pragma once

#include <QJsonDocument>
#include <QMultiMap>
#include <QObject>
#include <QString>
#include <QColor>

#include "Components/ActionComponent.h"
#include "Models/ActionsListModel.h"
#include "Models/AnimationSequenceModel.h"
#include "Models/PredefinedActionsModel.h"

struct SpriteSize {
    int Height;
    int Width;
};

struct TableSize {
    int Columns;
    int Rows;
};

struct AnimationSpriteInfo {
    uint SpriteId;
    int Column;
    int Row;
};

struct MessageSettings {
    int FontSize = 24;
    QColor MessageTextColor = "red";
};

struct SystemConfig {
    QString ImagePath = "qrc:/qml/Images/sprite-sheet.png";
    QString ChatBotWebSockURL = "localhost";
    int ChatBotWebSockPort = 656565;
    bool ChatBotAnyUser = false;
    bool Logging = false;
    float SpriteScale = 1.0f;
    int ScreenHeight = 1080;
    int ScreenWidth = 1920;
    QString TwitchTargetChannel;
    float ActionExecutionTime = 10.0f;
    float MovementSpeed = 3500.0f;
    float NextActionTimeout = 20.0f;
    MessageSettings Message;
};

class ConfigObject : public QObject {
    Q_OBJECT

public:
    explicit ConfigObject(QObject* Parent = nullptr);
    virtual ~ConfigObject();

    void ParseJsonDocument(const QJsonDocument& JsonDocument);
    void SaveConfigToFile(const QString& ConfigFile);

    Q_INVOKABLE void initActionsListModel(ActionsListModel* Model);
    Q_INVOKABLE void initAnimationsSequenceModel(AnimationSequenceModel* Model);
    Q_INVOKABLE void initPredefinedActionsListModel(PredefinedActionsListModel* Model);

    Q_INVOKABLE QString getSpriteSheetPath() const;
    Q_INVOKABLE QString getChatBotURL() const;
    Q_INVOKABLE int getChatBotPort() const;
    Q_INVOKABLE int getSpriteHeight() const;
    Q_INVOKABLE int getSpriteWidth() const;
    Q_INVOKABLE int getTableColumns() const;
    Q_INVOKABLE int getTableRows() const;
    Q_INVOKABLE float getSpriteScale() const;
    Q_INVOKABLE int getScreenHeight() const;
    Q_INVOKABLE int getScreenWidth() const;
    Q_INVOKABLE QString getTwitchTargeChannel() const;
    Q_INVOKABLE float getActionExecutionTime() const;
    Q_INVOKABLE QColor getMessageTextColor() const;
    Q_INVOKABLE float getMovementSpeed() const;
    Q_INVOKABLE float getNextActionTimeout() const;
    Q_INVOKABLE int getFontSize() const;
    Q_INVOKABLE bool getChatBotUser() const;
    Q_INVOKABLE bool getLogging() const;

    Q_INVOKABLE void saveConfig();
    Q_INVOKABLE void loadConfig();

    Q_INVOKABLE void saveSpriteSheetPath(const QString& ImagePath);
    Q_INVOKABLE void saveChatBotConfig(const QString& URL, const int Port);
    Q_INVOKABLE void saveSpriteWidth(const int Width);
    Q_INVOKABLE void saveSpriteHeight(const int Height);
    Q_INVOKABLE void saveTableColumns(const int Columns);
    Q_INVOKABLE void saveTableRows(const int Rows);
    Q_INVOKABLE void saveActions(const AnimationSequenceModel* Model);
    Q_INVOKABLE void saveSpriteScale(const float SpriteScale);
    Q_INVOKABLE void saveScreenResolution(const int Height, const int Width);
    Q_INVOKABLE void saveTargetTwitchChannel(const QString& TargetChannel);
    Q_INVOKABLE void savePredefinedActions(const PredefinedActionsListModel* Model);
    Q_INVOKABLE void saveActionExecutionTime(const float ActionExecutionTime);
    Q_INVOKABLE void saveMessageTextColor(const QColor& MessageTextColor);
    Q_INVOKABLE void saveMovementSpeed(const float MovementSpeed);
    Q_INVOKABLE void saveNextActionTimeout(const float NextActionTimeout);
    Q_INVOKABLE void saveFontSize(const int FontSize);
    Q_INVOKABLE void saveChatBotUser(const bool AnyUser);

    Q_INVOKABLE bool isConfigLoaded() const { return bConfigLoaded; }

    Q_INVOKABLE void setLoggerEnabled(bool Enabled);
    Q_INVOKABLE void saveDataToClipboard(const QString& Data);

    void SaveLogging(bool Logging);

signals:
    void actionsConfigured();
    void globalSettingsConfigured();
    void loggerEnabled(bool Enabled);

protected:
    void CopyImageToAssets();

    bool bConfigLoaded = false;
    SpriteSize SpriteSettings = {256, 256};
    TableSize TableSettings = {14, 4};
    SystemConfig SystemSettings;

    // List of the animations
    ActionsMap Map;

    // List of the predefined actions
    QVector<PredefinedAction> PredefinedActionsList;
};
