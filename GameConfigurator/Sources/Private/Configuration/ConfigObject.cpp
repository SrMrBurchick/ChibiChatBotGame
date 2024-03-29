#include "Configuration/ConfigObject.h"
#include "Managers/NotificationsManager.h"
#include "System/Logger.h"

#include <QClipboard>
#include <QFile>
#include <QGuiApplication>
#include <QImage>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>

// Global settings fields
constexpr char SOURCE_FILE[] = "source-file";

// System settings
constexpr char LOGGING[] = "logging";

// Screen resolution
constexpr char SCREEN_RESOLUTION[] = "screen-resolution";
constexpr char SCREEN_RESOLUTION_WIDTH[] = "width";
constexpr char SCREEN_RESOLUTION_HEIGHT[] = "height";

// Twitch target channel
constexpr char TWITCH_CHANNEL[] = "twitch-channel";

// Action execution time
constexpr char ACTION_EXECUTION_TIME[] = "action-execution-time";

// Message settings
constexpr char MESSAGE_SETTINGS[] = "message-settings";
constexpr char MESSAGE_TEXT_COLOR[] = "text-color";
constexpr char MESSAGE_FONT_SETTINGS[] = "font";
constexpr char MESSAGE_FONT_SIZE[] = "size";
constexpr char MESSAGE_FONT_TYPE[] = "type";

// Movement speed
constexpr char MOVEMENT_SPEED[] = "movement-speed";

// Next action timeout
constexpr char NEXT_ACTION_TIMEOUT[] = "next-action-timeout";

// Chat bot settings fields
constexpr char CHAT_BOT_SETTINGS[] = "chat-bot";
constexpr char CHAT_BOT_SETTINGS_URL[] = "url";
constexpr char CHAT_BOT_SETTINGS_PORT[] = "port";
constexpr char CHAT_BOT_USER[] = "any-user";

// Table settings fields
constexpr char TABLE_SETTINGS[] = "table-size";
constexpr char TABLE_SETTINGS_COLUMNS[] = "columns";
constexpr char TABLE_SETTINGS_ROWS[] = "rows";

// Sprite settings fields
constexpr char SPRITE_SETTINGS[] = "sprite-size";
constexpr char SPRITE_SETTINGS_HEIGHT[] = "height";
constexpr char SPRITE_SETTINGS_WIDTH[] = "width";
constexpr char SPRITE_SCALE[] = "sprite-scale";

// Animations settings fields
constexpr char ANIMATIONS_SETTINGS[] = "animations";
constexpr char ANIMATIONS_ITEM_COLUMN[] = "column";
constexpr char ANIMATIONS_ITEM_ROW[] = "row";
constexpr char ANIMATIONS_ITEM_INVERTED[] = "inverted";

// Predefined actions settings fields
constexpr char PREDEFINED_ACTIONS_SETTINGS[] = "predefined-actions";
constexpr char PREDEFINED_ACTIONS_ITEM_ACTION[] = "action";
constexpr char PREDEFINED_ACTIONS_ITEM_CHANCE[] = "chance";

ConfigObject::ConfigObject(QObject* Parent)
    : QObject(Parent)
{

}

ConfigObject::~ConfigObject()
{

}

float round(float Value) {
    return qRound(Value * 100.f) / 100.f;
}

void ConfigObject::ParseJsonDocument(const QJsonDocument& ConfigDocument)
{
    QVariantMap ConfigMap = ConfigDocument.toVariant().toMap();
    QVariantMap JsonTableSettings = ConfigMap[TABLE_SETTINGS].toMap();
    QVariantMap JsonSpriteSettings = ConfigMap[SPRITE_SETTINGS].toMap();
    QVariantMap JsonChatBotSettings = ConfigMap[CHAT_BOT_SETTINGS].toMap();
    QVariantMap JsonScreenResolution = ConfigMap[SCREEN_RESOLUTION].toMap();
    QVariantMap JsonMessageSettings = ConfigMap[MESSAGE_SETTINGS].toMap();
    QJsonArray JsonActionsArray = ConfigMap[ANIMATIONS_SETTINGS].toJsonArray();
    QJsonArray JsonPredefinedActions = ConfigMap[PREDEFINED_ACTIONS_SETTINGS].toJsonArray();

    // Init source file
    if (ConfigMap.contains(SOURCE_FILE)) {
        SystemSettings.ImagePath = ConfigMap[SOURCE_FILE].toString();
    }

    // Init sprite scale
    if (ConfigMap.contains(SPRITE_SCALE)) {
        SystemSettings.SpriteScale = round(ConfigMap[SPRITE_SCALE].toFloat());
    }

    // Init twitch channel
    if (ConfigMap.contains(TWITCH_CHANNEL)) {
        SystemSettings.TwitchTargetChannel = ConfigMap[TWITCH_CHANNEL].toString();
    }

    // Init action execution time
    if (ConfigMap.contains(ACTION_EXECUTION_TIME)) {
        SystemSettings.ActionExecutionTime = round(ConfigMap[ACTION_EXECUTION_TIME].toFloat());
    }

    // Init movement speed
    if (ConfigMap.contains(MOVEMENT_SPEED)) {
        SystemSettings.MovementSpeed =  round(ConfigMap[MOVEMENT_SPEED].toFloat());
    }

    // Init next action timeout
    if (ConfigMap.contains(NEXT_ACTION_TIMEOUT)) {
        SystemSettings.NextActionTimeout = round(ConfigMap[NEXT_ACTION_TIMEOUT].toFloat());
    }

    // Init message settings
    if (JsonMessageSettings.contains(MESSAGE_TEXT_COLOR) && JsonMessageSettings.contains(MESSAGE_FONT_SETTINGS)) {
        SystemSettings.Message.MessageTextColor = QColor(JsonMessageSettings[MESSAGE_TEXT_COLOR].toString());
        QVariantMap JsonFontSettings = JsonMessageSettings[MESSAGE_FONT_SETTINGS].toMap();
        if (JsonFontSettings.contains(MESSAGE_FONT_SIZE)) {
            SystemSettings.Message.FontSize = JsonFontSettings[MESSAGE_FONT_SIZE].toInt();
        }
    }

    // Init chat bot settings
    if (JsonChatBotSettings.contains(CHAT_BOT_SETTINGS_URL)
        && JsonChatBotSettings.contains(CHAT_BOT_SETTINGS_PORT)
        && JsonChatBotSettings.contains(CHAT_BOT_USER))
    {
        SystemSettings.ChatBotWebSockURL = JsonChatBotSettings[CHAT_BOT_SETTINGS_URL].toString();
        SystemSettings.ChatBotWebSockPort = JsonChatBotSettings[CHAT_BOT_SETTINGS_PORT].toInt();
        SystemSettings.ChatBotAnyUser = JsonChatBotSettings[CHAT_BOT_USER].toBool();
    }

    // Init sprite settings
    if (JsonSpriteSettings.contains(SPRITE_SETTINGS_WIDTH) && JsonSpriteSettings.contains(SPRITE_SETTINGS_HEIGHT)) {
        SpriteSettings.Height = JsonSpriteSettings[SPRITE_SETTINGS_HEIGHT].toInt();
        SpriteSettings.Width = JsonSpriteSettings[SPRITE_SETTINGS_WIDTH].toInt();
    }

    // Init table settings
    if (JsonTableSettings.contains(TABLE_SETTINGS_ROWS) && JsonTableSettings.contains(TABLE_SETTINGS_COLUMNS)) {
        TableSettings.Columns = JsonTableSettings[TABLE_SETTINGS_COLUMNS].toInt();
        TableSettings.Rows = JsonTableSettings[TABLE_SETTINGS_ROWS].toInt();
    }

    // Init screen settings
    if (JsonScreenResolution.contains(SCREEN_RESOLUTION_WIDTH) && JsonScreenResolution.contains(SCREEN_RESOLUTION_HEIGHT)) {
        SystemSettings.ScreenWidth = JsonScreenResolution[SCREEN_RESOLUTION_WIDTH].toInt();
        SystemSettings.ScreenHeight = JsonScreenResolution[SCREEN_RESOLUTION_HEIGHT].toInt();
    }

    // Init system settings
    if (ConfigMap.contains(LOGGING)) {
        SystemSettings.Logging = ConfigMap[LOGGING].toBool();
    }

    // Init animation map
    if (!JsonActionsArray.isEmpty()) {
        for (QJsonValueConstRef Item : JsonActionsArray) {
            QJsonObject Action = Item.toObject();
            QStringList ActionKeys = Action.keys();
            QString ActionName;
            QList<ActionSequenceSprite> AnimationSequence;
            if (ActionKeys.size() == 1) {
                ActionName = ActionKeys[0];
            }

            if (Action.contains(ActionName) && Action[ActionName].isArray()) {
                QJsonArray Sequence = Action[ActionName].toArray();
                for (QJsonValueConstRef Sprite : Sequence) {
                    if (!Sprite.isObject()) {
                        continue;
                    }

                    QJsonObject SpriteObj = Sprite.toObject();
                    ActionSequenceSprite SpriteInfo(0, 0);
                    SpriteInfo.Row = SpriteObj[ANIMATIONS_ITEM_ROW].toInt();
                    SpriteInfo.Column = SpriteObj[ANIMATIONS_ITEM_COLUMN].toInt();
                    SpriteInfo.bInverted = SpriteObj[ANIMATIONS_ITEM_INVERTED].toBool();

                    AnimationSequence.push_back(SpriteInfo);
                }

                Map[ActionName] = AnimationSequence;
            }
        }
    }

    // Init predefined actions
    if (!JsonPredefinedActions.isEmpty()) {
        for (QJsonValueConstRef Item : JsonPredefinedActions) {
            QJsonObject JsonAction = Item.toObject();
            PredefinedAction Action;
            Action.ActionName = JsonAction[PREDEFINED_ACTIONS_ITEM_ACTION].toString();
            Action.Chance = JsonAction[PREDEFINED_ACTIONS_ITEM_CHANCE].toInt();

            PredefinedActionsList.push_back(Action);
        }
    }

    bConfigLoaded = true;
}

void ConfigObject::SaveConfigToFile(const QString& ConfigFileName)
{
    QJsonObject JsonTableSettings;
    QJsonObject JsonSpriteSettings;
    QJsonObject JsonChatBotSettings;
    QJsonObject JsonMessageSettings;
    QJsonObject JsonMessageFontSettings;
    QJsonArray JsonActionsArray;
    QJsonObject JsonScreenResolution;
    QJsonArray JsonPredefinedActions;

    // Chat bot
    JsonChatBotSettings[CHAT_BOT_SETTINGS_URL] = SystemSettings.ChatBotWebSockURL;
    JsonChatBotSettings[CHAT_BOT_SETTINGS_PORT] = SystemSettings.ChatBotWebSockPort;
    JsonChatBotSettings[CHAT_BOT_USER] = SystemSettings.ChatBotAnyUser;

    // Table settings
    JsonTableSettings[TABLE_SETTINGS_ROWS] = TableSettings.Rows;
    JsonTableSettings[TABLE_SETTINGS_COLUMNS] = TableSettings.Columns;

    // Sprite settings
    JsonSpriteSettings[SPRITE_SETTINGS_WIDTH] = SpriteSettings.Width;
    JsonSpriteSettings[SPRITE_SETTINGS_HEIGHT] = SpriteSettings.Height;

    // Screen resolution
    JsonScreenResolution[SCREEN_RESOLUTION_HEIGHT] = SystemSettings.ScreenHeight;
    JsonScreenResolution[SCREEN_RESOLUTION_WIDTH] = SystemSettings.ScreenWidth;

    // Message settings
    JsonMessageSettings[MESSAGE_TEXT_COLOR] = SystemSettings.Message.MessageTextColor.name();
    JsonMessageFontSettings[MESSAGE_FONT_SIZE] = SystemSettings.Message.FontSize;
    JsonMessageSettings[MESSAGE_FONT_SETTINGS] = JsonMessageFontSettings;

    // Actions map
    for (auto Iter = Map.begin(); Iter != Map.end(); ++Iter) {
        QJsonObject JsonAction;
        QJsonArray JsonActionSprites;
        for (const ActionSequenceSprite& Sprite : Iter.value()) {
            QJsonObject JsonActionSprite;
            JsonActionSprite[ANIMATIONS_ITEM_COLUMN] = Sprite.Column;
            JsonActionSprite[ANIMATIONS_ITEM_ROW] = Sprite.Row;
            JsonActionSprite[ANIMATIONS_ITEM_INVERTED] = Sprite.bInverted;
            JsonActionSprites.push_back(JsonActionSprite);
        }
        JsonAction[Iter.key()] = JsonActionSprites;
        JsonActionsArray.push_back(JsonAction);
    }

    // Predefined actions
    for (const PredefinedAction& Action : PredefinedActionsList) {
        QJsonObject JsonAction;
        JsonAction[PREDEFINED_ACTIONS_ITEM_ACTION] = Action.ActionName;
        JsonAction[PREDEFINED_ACTIONS_ITEM_CHANCE] = Action.Chance;
        JsonPredefinedActions.push_back(JsonAction);
    }

    QJsonObject Config;
    Config[LOGGING] = SystemSettings.Logging;
    Config[SOURCE_FILE] = SystemSettings.ImagePath;
    Config[ACTION_EXECUTION_TIME] = SystemSettings.ActionExecutionTime;
    Config[MESSAGE_SETTINGS] = JsonMessageSettings;
    Config[SPRITE_SCALE] = SystemSettings.SpriteScale;
    Config[MOVEMENT_SPEED] = SystemSettings.MovementSpeed;
    Config[NEXT_ACTION_TIMEOUT] = SystemSettings.NextActionTimeout;
    Config[CHAT_BOT_SETTINGS] = JsonChatBotSettings;
    Config[TABLE_SETTINGS] = JsonTableSettings;
    Config[SPRITE_SETTINGS] = JsonSpriteSettings;
    Config[SCREEN_RESOLUTION] = JsonScreenResolution;
    Config[TWITCH_CHANNEL] = SystemSettings.TwitchTargetChannel;
    Config[PREDEFINED_ACTIONS_SETTINGS] = JsonPredefinedActions;
    Config[ANIMATIONS_SETTINGS] = JsonActionsArray;

    QFile ConfigFile(ConfigFileName);
    if (ConfigFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QJsonDocument ConfigDocument = QJsonDocument(Config);
        ConfigFile.write(ConfigDocument.toJson());
        ConfigFile.close();
        NotificationsManager::SendNotification("Config", "Saved successfully!");
    } else {
        NotificationsManager::SendNotification("Config", "Failed to save config!");
    }
}

void ConfigObject::saveConfig()
{
    SaveConfigToFile(QT_STRINGIFY(CONFIG_FILE));
    bConfigLoaded = true;
}

void ConfigObject::loadConfig()
{
    QFile ConfigFile(QT_STRINGIFY(CONFIG_FILE));
    if (ConfigFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QJsonDocument JsonConfig = QJsonDocument::fromJson(ConfigFile.readAll());
        ParseJsonDocument(JsonConfig);
        ConfigFile.close();
        emit loggerEnabled(SystemSettings.Logging);
    }
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
    CopyImageToAssets();
}

void ConfigObject::initActionsListModel(ActionsListModel* Model)
{
    if (!isConfigLoaded() && Model == nullptr) {
        return;
    }

    for (const QString& Key : Map.keys()) {
        Model->addNewAction(Key);
    }
}

void ConfigObject::initAnimationsSequenceModel(AnimationSequenceModel* Model)
{
    if (!isConfigLoaded()) {
        return;
    }

    if (Model != nullptr) {
        Model->initModel(Map);
    }
}

QString ConfigObject::getSpriteSheetPath() const
{
    return SystemSettings.ImagePath;
}

QString ConfigObject::getChatBotURL() const
{
    return SystemSettings.ChatBotWebSockURL;
}

int ConfigObject::getChatBotPort() const
{
    return SystemSettings.ChatBotWebSockPort;
}

int ConfigObject::getSpriteHeight() const
{
    return SpriteSettings.Height;
}

int ConfigObject::getSpriteWidth() const
{
    return SpriteSettings.Width;
}

int ConfigObject::getTableColumns() const
{
    return TableSettings.Columns;
}

int ConfigObject::getTableRows() const
{
    return TableSettings.Rows;
}

void ConfigObject::CopyImageToAssets()
{
    QImage image;
    if (SystemSettings.ImagePath.isEmpty()) {
        return;
    }

    QString SystemPath = SystemSettings.ImagePath;
#ifdef Q_OS_WIN
    SystemPath.remove("file:///");
#else
    SystemPath.remove("file://");
#endif
    if (image.load(SystemPath)) {
        image.save(QT_STRINGIFY(GAME_ASSET_IMAGE_PATH));
    }
}

void ConfigObject::saveSpriteScale(const float SpriteScale)
{
    SystemSettings.SpriteScale = SpriteScale;
}

float ConfigObject::getSpriteScale() const
{
    return SystemSettings.SpriteScale;
}

void ConfigObject::saveScreenResolution(const int Height, const int Width)
{
    SystemSettings.ScreenHeight = Height;
    SystemSettings.ScreenWidth = Width;
}

void ConfigObject::saveTargetTwitchChannel(const QString& TargetChannel)
{
    SystemSettings.TwitchTargetChannel = TargetChannel;
}

int ConfigObject::getScreenHeight() const
{
    return SystemSettings.ScreenHeight;
}

int ConfigObject::getScreenWidth() const
{
    return SystemSettings.ScreenWidth;
}

QString ConfigObject::getTwitchTargeChannel() const
{
    return SystemSettings.TwitchTargetChannel;
}

void ConfigObject::savePredefinedActions(const PredefinedActionsListModel* Model)
{
    if (Model != nullptr) {
        PredefinedActionsList = Model->getList();
    }
}

void ConfigObject::initPredefinedActionsListModel(PredefinedActionsListModel* Model)
{
    if (!isConfigLoaded()) {
        return;
    }

    if (Model != nullptr) {
        Model->initModel(PredefinedActionsList);
    }
}

float ConfigObject::getActionExecutionTime() const
{
    return SystemSettings.ActionExecutionTime;
}

QColor ConfigObject::getMessageTextColor() const
{
    return SystemSettings.Message.MessageTextColor;
}

void ConfigObject::saveActionExecutionTime(const float ActionExecutionTime)
{
    SystemSettings.ActionExecutionTime = ActionExecutionTime;
}

void ConfigObject::saveMessageTextColor(const QColor& MessageTextColor)
{
    SystemSettings.Message.MessageTextColor = MessageTextColor;
}

float ConfigObject::getMovementSpeed() const
{
    return SystemSettings.MovementSpeed;
}

void ConfigObject::saveMovementSpeed(const float MovementSpeed)
{
    SystemSettings.MovementSpeed = MovementSpeed;
}

float ConfigObject::getNextActionTimeout() const
{
    return SystemSettings.NextActionTimeout;
}

void ConfigObject::saveNextActionTimeout(const float NextActionTimeout)
{
    SystemSettings.NextActionTimeout = NextActionTimeout;
}

int ConfigObject::getFontSize() const
{
    return SystemSettings.Message.FontSize;
}

void ConfigObject::saveFontSize(const int FontSize)
{
    SystemSettings.Message.FontSize = FontSize;
}

void ConfigObject::setLoggerEnabled(bool Enabled)
{
    emit loggerEnabled(Enabled);
}

void ConfigObject::saveDataToClipboard(const QString& Data)
{
    if (QClipboard* Clipboard = QGuiApplication::clipboard()) {
        Clipboard->setText(Data);
        NotificationsManager::SendNotification(
            "System Clipboard",
            QString::asprintf(
                "Data %s was saved \nto the system clipboard",
                Data.toStdString().c_str()
            )
        );
    }
}

bool ConfigObject::getChatBotUser() const
{
    return SystemSettings.ChatBotAnyUser;
}

void ConfigObject::saveChatBotUser(const bool AnyUser)
{
    SystemSettings.ChatBotAnyUser = AnyUser;
}

void ConfigObject::SaveLogging(bool Logging)
{
    SystemSettings.Logging = Logging;
}

bool ConfigObject::getLogging() const
{
    return SystemSettings.Logging;
}
