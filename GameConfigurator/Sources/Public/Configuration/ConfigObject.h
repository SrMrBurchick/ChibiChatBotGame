#pragma once

#include <QJsonDocument>
#include <QMultiMap>
#include <QObject>
#include <QString>

#include "Components/ActionComponent.h"
#include "Models/AnimationSequenceModel.h"

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
};

class ConfigObject : public QObject {
    Q_OBJECT

    Q_PROPERTY(bool configLoaded READ isConfigLoaded)
public:
    explicit ConfigObject(QObject* Parent = nullptr);
    virtual ~ConfigObject();

    void ParseJsonDocument(const QJsonDocument& JsonDocument);
    void SaveConfigToFile(const QString& ConfigFile);

    Q_INVOKABLE void saveConfig();
    Q_INVOKABLE void loadConfig();

    Q_INVOKABLE void saveSpriteSheetPath(const QString& ImagePath);
    Q_INVOKABLE void saveChatBotConfig(const QString& URL, const int Port);
    Q_INVOKABLE void saveSpriteSettings(const int Width, const int Height);
    Q_INVOKABLE void saveTableSettings(const int Columns, const int Rows);
    Q_INVOKABLE void saveActions(const AnimationSequenceModel* Model);

public slots:
    bool isConfigLoaded() const { return bConfigLoaded; }

protected:
    bool bConfigLoaded = false;
    QString SpriteSheetPath;
    SpriteSize SpriteSettings;
    TableSize TableSettings;
    SystemConfig SystemSettings;

    // List of the animations
    ActionsMap Map;
};
