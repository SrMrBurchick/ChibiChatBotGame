#pragma once

#include <QJsonDocument>
#include <QMultiMap>
#include <QObject>
#include <QString>

#include "Components/ActionComponent.h"
#include "Models/AnimationSequenceModel.h"
#include "Models/ActionsListModel.h"

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

struct SystemConfig {
    QString ImagePath;
    QString ChatBotWebSockURL;
    int ChatBotWebSockPort;
    float SpriteScale;
    int ScreenHeight;
    int ScreenWidth;
    QString TwitchTargetChannel;
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

    Q_INVOKABLE void saveConfig();
    Q_INVOKABLE void loadConfig();

    Q_INVOKABLE void saveSpriteSheetPath(const QString& ImagePath);
    Q_INVOKABLE void saveChatBotConfig(const QString& URL, const int Port);
    Q_INVOKABLE void saveSpriteSettings(const int Width, const int Height);
    Q_INVOKABLE void saveTableSettings(const int Columns, const int Rows);
    Q_INVOKABLE void saveActions(const AnimationSequenceModel* Model);
    Q_INVOKABLE void saveSpriteScale(const float SpriteScale);
    Q_INVOKABLE void saveScreenResolution(const int Height, const int Width);
    Q_INVOKABLE void saveTargetTwitchChannel(const QString& TargetChannel);

    Q_INVOKABLE bool isConfigLoaded() const { return bConfigLoaded; }

protected:
    void CopyImageToAssets();

    bool bConfigLoaded = false;
    QString SpriteSheetPath;
    SpriteSize SpriteSettings;
    TableSize TableSettings;
    SystemConfig SystemSettings;

    // List of the animations
    ActionsMap Map;
};
