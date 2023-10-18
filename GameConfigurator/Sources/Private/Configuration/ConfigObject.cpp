#include "Configuration/ConfigObject.h"

#include <QFile>
#include <QJsonObject>
#include <QJsonArray>

ConfigObject::ConfigObject(QObject* Parent)
    : QObject(Parent)
{

}

ConfigObject::~ConfigObject()
{

}

void ConfigObject::ParseJsonDocument(const QJsonDocument& ConfigDocument)
{
    // TODO: Parse loaded json config
}

void ConfigObject::SaveConfigToFile(const QString& ConfigFileName)
{

    QJsonObject JsonTableSettings;
    QJsonObject JsonSpriteSettings;
    QJsonObject JsonChatBotSettings;
    QJsonArray JsonActionsArray;

    // Chat bot
    JsonChatBotSettings["url"] = SystemSettings.ChatBotWebSockURL;
    JsonChatBotSettings["port"] = SystemSettings.ChatBotWebSockPort;

    // Table settings
    JsonTableSettings["rows"] = TableSettings.Rows;
    JsonTableSettings["columns"] = TableSettings.Columns;

    // Sprite settings
    JsonSpriteSettings["width"] = SpriteSettings.Width;
    JsonSpriteSettings["height"] = SpriteSettings.Height;

    // Actions map
    for (auto Iter = Map.begin(); Iter != Map.end(); ++Iter) {
        QJsonObject JsonAction;
        QJsonArray JsonActionSprites;
        for (const ActionSequenceSprite& Sprite : Iter.value()) {
            QJsonObject JsonActionSprite;
            JsonActionSprite["column"] = Sprite.Column;
            JsonActionSprite["row"] = Sprite.Row;
            JsonActionSprite["inverted"] = Sprite.bInverted;
            JsonActionSprites.push_back(JsonActionSprite);
        }
        JsonAction[Iter.key()] = JsonActionSprites;
        JsonActionsArray.push_back(JsonAction);
    }

    QJsonObject Config;
    Config["source-file"] = SystemSettings.ImagePath;
    Config["chat-bot"] = JsonChatBotSettings;
    Config["table-size"] = JsonTableSettings;
    Config["sprite-size"] = JsonSpriteSettings;
    Config["animations"] = JsonActionsArray;

    QFile ConfigFile(ConfigFileName);
    if (ConfigFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
        ConfigFile.write(QJsonDocument(Config).toJson());
        ConfigFile.close();
    }

}

void ConfigObject::saveConfig()
{
    SaveConfigToFile("ChibiChatBotGameConfig.json");
}

void ConfigObject::loadConfig()
{

}

void ConfigObject::saveChatBotConfig(const QString& URL, const int Port)
{
    SystemSettings.ChatBotWebSockURL = URL;
    SystemSettings.ChatBotWebSockPort = Port;
}

void ConfigObject::saveSpriteSettings(const int Width, const int Height)
{
    SpriteSettings.Width = Width;
    SpriteSettings.Height = Height;
}

void ConfigObject::saveTableSettings(const int Columns, const int Rows)
{
    TableSettings.Columns = Columns;
    TableSettings.Rows = Rows;
}

void ConfigObject::saveActions(const AnimationSequenceModel* Model)
{
    if (Model != nullptr) {
        Map = Model->getMap();
    }
}

void ConfigObject::saveSpriteSheetPath(const QString& ImagePath)
{
    SystemSettings.ImagePath = ImagePath;
}
