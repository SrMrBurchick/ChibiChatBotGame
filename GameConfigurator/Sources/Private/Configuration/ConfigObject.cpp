#include "Configuration/ConfigObject.h"
#include "Managers/NotificationsManager.h"
#include "System/Logger.h"
#include "Managers/ActionsManager.h"

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
constexpr char TWITCH_SETTINGS[] = "twitch-settings";
constexpr char TWITCH_BOT_SETTINGS[] = "twitch-bot-settings";
constexpr char TWITCH_CHANNEL[] = "twitch-channel";
constexpr char TWITCH_OAUTH[] = "oauth-token";
constexpr char TWITCH_CLIENT_ID[] = "client-id";
constexpr char TWITCH_USER_ID[] = "user-id";

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
constexpr char BOT_SETTINGS[] = "bot-settings";
constexpr char BOT_SETTINGS_URL[] = "url";
constexpr char BOT_SETTINGS_PORT[] = "port";
constexpr char BOT_USER[] = "any-user";

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

// Action config
constexpr char ACTIONS_SETTINGS[] = "actions";
constexpr char ACTION_NAME[] = "name";
constexpr char ACTION_CONFIG_SETTINGS[] = "config";
constexpr char ACTION_CONFIG_INTERRUPT[] = "can-interrupt";
constexpr char ACTION_CONFIG_TWITCH_REWARD_ID[] = "twitch-reward-id";
constexpr char ACTION_CONFIG_TWITCH_EVENT_TYPE[] = "twitch-event-type";
constexpr char ACTION_CONFIG_TEXT_SETTINGS[] = "text-settings";
constexpr char ACTION_CONFIG_TEXT_MESSAGE[] = "message";
constexpr char ACTION_CONFIG_TEXT_DISPLAY_TIME[] = "display-time";
constexpr char ACTION_CONFIG_TEXT_COLOR[] = "color";
constexpr char ACTION_CONFIG_TEXT_SIZE[] = "size";

ConfigObject::ConfigObject(QObject* Parent)
    : QObject(Parent)
{

}

ConfigObject::~ConfigObject()
{
    saveConfig();
}

float round(float Value) {
    return qRound(Value * 100.f) / 100.f;
}

void ConfigObject::ParseJsonDocument(const QJsonDocument& ConfigDocument)
{
    QVariantMap ConfigMap = ConfigDocument.toVariant().toMap();
    QVariantMap JsonTableSettings = ConfigMap[TABLE_SETTINGS].toMap();
    QVariantMap JsonSpriteSettings = ConfigMap[SPRITE_SETTINGS].toMap();
    QVariantMap JsonTwitchSettings = ConfigMap[TWITCH_SETTINGS].toMap();
    QVariantMap JsonScreenResolution = ConfigMap[SCREEN_RESOLUTION].toMap();
    QVariantMap JsonMessageSettings = ConfigMap[MESSAGE_SETTINGS].toMap();
    QJsonArray JsonPredefinedActions = ConfigMap[PREDEFINED_ACTIONS_SETTINGS].toJsonArray();

    // Init source file
    if (ConfigMap.contains(SOURCE_FILE)) {
        SystemSettings.ImagePath = ConfigMap[SOURCE_FILE].toString();
    }

    // Init sprite scale
    if (ConfigMap.contains(SPRITE_SCALE)) {
        SystemSettings.Game.SpriteScale = round(ConfigMap[SPRITE_SCALE].toFloat());
    }

    // Init twitch settings
    if (JsonTwitchSettings.contains(TWITCH_CHANNEL) && JsonTwitchSettings.contains(TWITCH_OAUTH)) {
        SystemSettings.Twitch.ChannelName = JsonTwitchSettings[TWITCH_CHANNEL].toString();
        SystemSettings.Twitch.OAuthToken = JsonTwitchSettings[TWITCH_OAUTH].toString();
        if (JsonTwitchSettings.contains(TWITCH_BOT_SETTINGS)) {
            QVariantMap JsonTwitchBotSettings = JsonTwitchSettings[TWITCH_BOT_SETTINGS].toMap();
            SystemSettings.Twitch.Bot.WebSockURL = JsonTwitchBotSettings[BOT_SETTINGS_URL].toString();
            SystemSettings.Twitch.Bot.WebSockPort = JsonTwitchBotSettings[BOT_SETTINGS_PORT].toInt();
            SystemSettings.Twitch.Bot.ChatAnyUser = JsonTwitchBotSettings[BOT_USER].toInt();
        }

    }

    // Init action execution time
    if (ConfigMap.contains(ACTION_EXECUTION_TIME)) {
        SystemSettings.Game.ActionExecutionTime = round(ConfigMap[ACTION_EXECUTION_TIME].toFloat());
    }

    // Init movement speed
    if (ConfigMap.contains(MOVEMENT_SPEED)) {
        SystemSettings.Game.MovementSpeed =  round(ConfigMap[MOVEMENT_SPEED].toFloat());
    }

    // Init next action timeout
    if (ConfigMap.contains(NEXT_ACTION_TIMEOUT)) {
        SystemSettings.Game.NextActionTimeout = round(ConfigMap[NEXT_ACTION_TIMEOUT].toFloat());
    }

    // Init message settings
    if (JsonMessageSettings.contains(MESSAGE_TEXT_COLOR) && JsonMessageSettings.contains(MESSAGE_FONT_SETTINGS)) {
        SystemSettings.Message.MessageTextColor = QColor(JsonMessageSettings[MESSAGE_TEXT_COLOR].toString());
        QVariantMap JsonFontSettings = JsonMessageSettings[MESSAGE_FONT_SETTINGS].toMap();
        if (JsonFontSettings.contains(MESSAGE_FONT_SIZE)) {
            SystemSettings.Message.FontSize = JsonFontSettings[MESSAGE_FONT_SIZE].toInt();
        }
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
        SystemSettings.Game.ScreenWidth = JsonScreenResolution[SCREEN_RESOLUTION_WIDTH].toInt();
        SystemSettings.Game.ScreenHeight = JsonScreenResolution[SCREEN_RESOLUTION_HEIGHT].toInt();
    }

    // Init system settings
    if (ConfigMap.contains(LOGGING)) {
        SystemSettings.Logging = ConfigMap[LOGGING].toBool();
    }

    // Init actions
    if (ConfigMap.contains(ACTIONS_SETTINGS)) {
        ActionsArray = ConfigMap[ACTIONS_SETTINGS].toJsonArray();
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
    QJsonObject JsonTwitchSettings;
    QJsonObject JsonTwitchBotSettings;
    QJsonObject JsonMessageSettings;
    QJsonObject JsonMessageFontSettings;
    QJsonArray JsonActionsArray;
    QJsonObject JsonScreenResolution;
    QJsonArray JsonPredefinedActions;

    // Twitch settings
    JsonTwitchSettings[TWITCH_CHANNEL] = SystemSettings.Twitch.ChannelName;
    JsonTwitchSettings[TWITCH_OAUTH] = SystemSettings.Twitch.OAuthToken;
    JsonTwitchSettings[TWITCH_USER_ID] = SystemSettings.Twitch.UserId;
    JsonTwitchSettings[TWITCH_CLIENT_ID] = QT_STRINGIFY(CLIENT_ID);
    JsonTwitchBotSettings[BOT_SETTINGS_URL] = SystemSettings.Twitch.Bot.WebSockURL;
    JsonTwitchBotSettings[BOT_SETTINGS_PORT] = SystemSettings.Twitch.Bot.WebSockPort;
    JsonTwitchBotSettings[BOT_USER] = SystemSettings.Twitch.Bot.ChatAnyUser;
    JsonTwitchSettings[BOT_SETTINGS] = JsonTwitchBotSettings;

    // Table settings
    JsonTableSettings[TABLE_SETTINGS_ROWS] = TableSettings.Rows;
    JsonTableSettings[TABLE_SETTINGS_COLUMNS] = TableSettings.Columns;

    // Sprite settings
    JsonSpriteSettings[SPRITE_SETTINGS_WIDTH] = SpriteSettings.Width;
    JsonSpriteSettings[SPRITE_SETTINGS_HEIGHT] = SpriteSettings.Height;

    // Screen resolution
    JsonScreenResolution[SCREEN_RESOLUTION_HEIGHT] = SystemSettings.Game.ScreenHeight;
    JsonScreenResolution[SCREEN_RESOLUTION_WIDTH] = SystemSettings.Game.ScreenWidth;

    // Message settings
    JsonMessageSettings[MESSAGE_TEXT_COLOR] = SystemSettings.Message.MessageTextColor.name();
    JsonMessageFontSettings[MESSAGE_FONT_SIZE] = SystemSettings.Message.FontSize;
    JsonMessageSettings[MESSAGE_FONT_SETTINGS] = JsonMessageFontSettings;

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
    Config[ACTION_EXECUTION_TIME] = SystemSettings.Game.ActionExecutionTime;
    Config[MESSAGE_SETTINGS] = JsonMessageSettings;
    Config[SPRITE_SCALE] = SystemSettings.Game.SpriteScale;
    Config[MOVEMENT_SPEED] = SystemSettings.Game.MovementSpeed;
    Config[NEXT_ACTION_TIMEOUT] = SystemSettings.Game.NextActionTimeout;
    Config[TWITCH_SETTINGS] = JsonTwitchSettings;
    Config[TABLE_SETTINGS] = JsonTableSettings;
    Config[SPRITE_SETTINGS] = JsonSpriteSettings;
    Config[SCREEN_RESOLUTION] = JsonScreenResolution;
    Config[PREDEFINED_ACTIONS_SETTINGS] = JsonPredefinedActions;
    Config[ACTIONS_SETTINGS] = ActionsArray;

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
    SetBusy(true);
    SaveConfigToFile(QT_STRINGIFY(CONFIG_FILE));
    bConfigLoaded = true;
    SetBusy(false);
}

void ConfigObject::loadConfig()
{
    SetBusy(true);
    QFile ConfigFile(QT_STRINGIFY(CONFIG_FILE));
    if (ConfigFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QJsonDocument JsonConfig = QJsonDocument::fromJson(ConfigFile.readAll());
        ParseJsonDocument(JsonConfig);
        ConfigFile.close();
        emit loggerEnabled(SystemSettings.Logging);
        NotificationsManager::SendNotification("Config", "Loaded successfully!");
    } else {
        NotificationsManager::SendNotification("Config", QString("Failed to load. %1").arg(ConfigFile.errorString()));
    }
    SetBusy(false);
}

void ConfigObject::saveBotConfig(const QString& URL, const int Port)
{
    SystemSettings.Twitch.Bot.WebSockURL = URL;
    SystemSettings.Twitch.Bot.WebSockPort = Port;
}

void ConfigObject::saveSpriteWidth(const int Width)
{
    SpriteSettings.Width = Width;
}

void ConfigObject::saveSpriteHeight(const int Height)
{
    SpriteSettings.Height = Height;
}

void ConfigObject::saveTableColumns(const int Columns)
{
    TableSettings.Columns = Columns;
}

void ConfigObject::saveTableRows(const int Rows)
{
    TableSettings.Rows = Rows;
}

void ConfigObject::saveSpriteSheetPath(const QString& ImagePath)
{
    SystemSettings.ImagePath = ImagePath;
    CopyImageToAssets();
}

QString ConfigObject::getSpriteSheetPath() const
{
    return SystemSettings.ImagePath;
}

QString ConfigObject::getBotURL() const
{
    return SystemSettings.Twitch.Bot.WebSockURL;
}

int ConfigObject::getBotPort() const
{
    return SystemSettings.Twitch.Bot.WebSockPort;
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
    SystemSettings.Game.SpriteScale = SpriteScale;
}

float ConfigObject::getSpriteScale() const
{
    return SystemSettings.Game.SpriteScale;
}

void ConfigObject::saveScreenResolution(const int Height, const int Width)
{
    SystemSettings.Game.ScreenHeight = Height;
    SystemSettings.Game.ScreenWidth = Width;
}

void ConfigObject::saveTargetTwitchChannel(const QString& TargetChannel)
{
    SystemSettings.Twitch.ChannelName = TargetChannel;
}

int ConfigObject::getScreenHeight() const
{
    return SystemSettings.Game.ScreenHeight;
}

int ConfigObject::getScreenWidth() const
{
    return SystemSettings.Game.ScreenWidth;
}

QString ConfigObject::getTwitchTargeChannel() const
{
    return SystemSettings.Twitch.ChannelName;
}

void ConfigObject::savePredefinedActions(const PredefinedActionsListModel* Model)
{
    if (Model != nullptr) {
        PredefinedActionsList = Model->getList();
    }
}

float ConfigObject::getActionExecutionTime() const
{
    return SystemSettings.Game.ActionExecutionTime;
}

QColor ConfigObject::getMessageTextColor() const
{
    return SystemSettings.Message.MessageTextColor;
}

void ConfigObject::saveActionExecutionTime(const float ActionExecutionTime)
{
    SystemSettings.Game.ActionExecutionTime = ActionExecutionTime;
}

void ConfigObject::saveMessageTextColor(const QColor& MessageTextColor)
{
    SystemSettings.Message.MessageTextColor = MessageTextColor;
}

float ConfigObject::getMovementSpeed() const
{
    return SystemSettings.Game.MovementSpeed;
}

void ConfigObject::saveMovementSpeed(const float MovementSpeed)
{
    SystemSettings.Game.MovementSpeed = MovementSpeed;
}

float ConfigObject::getNextActionTimeout() const
{
    return SystemSettings.Game.NextActionTimeout;
}

void ConfigObject::saveNextActionTimeout(const float NextActionTimeout)
{
    SystemSettings.Game.NextActionTimeout = NextActionTimeout;
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
    return SystemSettings.Twitch.Bot.ChatAnyUser;
}

void ConfigObject::SaveLogging(bool Logging)
{
    SystemSettings.Logging = Logging;
}

bool ConfigObject::getLogging() const
{
    return SystemSettings.Logging;
}

void ConfigObject::saveTwitchInfo(const QString& ChannelName, const QString& OAuthToken, const QString& UserId)
{
    SystemSettings.Twitch.ChannelName = ChannelName;
    SystemSettings.Twitch.OAuthToken = OAuthToken;
    SystemSettings.Twitch.UserId = UserId;
}

void ConfigObject::SaveActions(const QVector<QSharedPointer<Action>>& Actions)
{
    if (ActionsArray.count() > 0) {
        ActionsArray = QJsonArray();
    }

    for (const QSharedPointer<Action>& Action : Actions) {
        QJsonObject JsonAction;
        JsonAction[ACTION_NAME] = Action->getName();

        // Config
        QJsonObject JsonActionConfig;
        if (ActionConfig* Config = Action->getConfig()) {
            QJsonObject TextConfig;
            JsonActionConfig[ACTION_CONFIG_INTERRUPT] = Config->bCanInterrupt;
            JsonActionConfig[ACTION_CONFIG_TWITCH_REWARD_ID] = Config->ChannelPointsRewardID;
            JsonActionConfig[ACTION_CONFIG_TWITCH_EVENT_TYPE] = Config->EventType;

            TextConfig[ACTION_CONFIG_TEXT_COLOR] = Config->TextColor.name();
            TextConfig[ACTION_CONFIG_TEXT_DISPLAY_TIME] = Config->DisplayTime;
            TextConfig[ACTION_CONFIG_TEXT_SIZE] = Config->FontSize;
            TextConfig[ACTION_CONFIG_TEXT_MESSAGE] = Config->Text;

            JsonActionConfig[ACTION_CONFIG_TEXT_SETTINGS] = TextConfig;
        }

        JsonAction[ACTION_CONFIG_SETTINGS] = JsonActionConfig;

        // Animations
        QJsonArray Animations;
        for (int i = 0; i < Action->getTotalSpritesCounts(); ++i) {
            ActionSequenceSprite Sprite = Action->GetSpriteById(i);
            QJsonObject JsonSprite;
            JsonSprite[ANIMATIONS_ITEM_INVERTED] = Sprite.bInverted;
            JsonSprite[ANIMATIONS_ITEM_COLUMN] = Sprite.Column;
            JsonSprite[ANIMATIONS_ITEM_ROW] = Sprite.Row;

            Animations.push_back(JsonSprite);
        }

        JsonAction[ANIMATIONS_SETTINGS] = Animations;
        ActionsArray.push_back(JsonAction);
    }
}

void ConfigObject::InitActionsManager(ActionsManager* Manager) const
{
    if (!Manager) {
        LOG_WARNING("ConfigObject: Invalid Manager to init");
        return;
    }

    for (QJsonValueConstRef JsonActionRef : ActionsArray) {
        QJsonObject JsonAction = JsonActionRef.toObject();
        QSharedPointer<Action> Action = Manager->CreateNewAction(JsonAction[ACTION_NAME].toString());
        if (!Action.isNull()) {
            // Config
            if (ActionConfig* Config = Action->getConfig()) {
                if (JsonAction.contains(ACTION_CONFIG_SETTINGS)) {
                    QJsonObject JsonActionConfig = JsonAction[ACTION_CONFIG_SETTINGS].toObject();
                    if (JsonActionConfig.contains(ACTION_CONFIG_TWITCH_REWARD_ID)
                        && JsonActionConfig.contains(ACTION_CONFIG_INTERRUPT)
                        && JsonActionConfig.contains(ACTION_CONFIG_TWITCH_EVENT_TYPE)) {
                        Config->bCanInterrupt = JsonActionConfig[ACTION_CONFIG_INTERRUPT].toBool();
                        Config->ChannelPointsRewardID = JsonActionConfig[ACTION_CONFIG_TWITCH_REWARD_ID].toString();
                        Config->EventType = JsonActionConfig[ACTION_CONFIG_TWITCH_EVENT_TYPE].toString();

                        if (JsonActionConfig.contains(ACTION_CONFIG_TEXT_SETTINGS)) {
                            QJsonObject ActionTextConfig = JsonActionConfig[ACTION_CONFIG_TEXT_SETTINGS].toObject();
                            if (ActionTextConfig.contains(ACTION_CONFIG_TEXT_MESSAGE)) {
                                Config->Text = ActionTextConfig[ACTION_CONFIG_TEXT_MESSAGE].toString();
                            }
                            if (ActionTextConfig.contains(ACTION_CONFIG_TEXT_SIZE)) {
                                Config->FontSize = ActionTextConfig[ACTION_CONFIG_TEXT_SIZE].toInt();
                            }
                            if (ActionTextConfig.contains(ACTION_CONFIG_TEXT_DISPLAY_TIME)) {
                                Config->DisplayTime = ActionTextConfig[ACTION_CONFIG_TEXT_DISPLAY_TIME].toInt();
                            }
                            if (ActionTextConfig.contains(ACTION_CONFIG_TEXT_COLOR)) {
                                Config->TextColor = QColor(ActionTextConfig[ACTION_CONFIG_TEXT_COLOR].toString());
                            }
                        }
                    }
                }
            }

            // Animations
            QJsonArray Animations;
            if (JsonAction.contains(ANIMATIONS_SETTINGS)) {
                Animations = JsonAction[ANIMATIONS_SETTINGS].toArray();
            }

            for (QJsonValueConstRef AnimationRef : Animations) {
                QJsonObject Animation = AnimationRef.toObject();
                if (Animation.contains(ANIMATIONS_ITEM_COLUMN)
                    && Animation.contains(ANIMATIONS_ITEM_ROW)
                    && Animation.contains(ANIMATIONS_ITEM_INVERTED)) {

                }

                if (ActionSequenceSprite* NewSprite = Action->CreateNewSprite(Animation[ANIMATIONS_ITEM_COLUMN].toInt(), Animation[ANIMATIONS_ITEM_ROW].toInt())) {
                    NewSprite->bInverted = Animation[ANIMATIONS_ITEM_INVERTED].toBool();
                }
            }
        }
    }
}

void ConfigObject::SetBusy(bool isBusy)
{
    bIsBusy = isBusy;
    emit busyUpdated();
}
