#include "Core/Twitch/ChannelPointsReward.h"

#include "System/Logger.h"

#include <QJsonDocument>
#include <QJsonValue>
#include <QNetworkReply>
#include <QByteArray>
#include <QJsonObject>
#include <QJsonArray>

constexpr char TWITCH_RESPONSE_DATA[] = "data";

constexpr char CHANNEL_POINT_ID[] = "id";
constexpr char CHANNEL_POINT_IMAGE[] = "image";
constexpr char CHANNEL_POINT_BACKGROUND_COLOR[] = "background_color";
constexpr char CHANNEL_POINT_ENABLED[] = "is_enabled";
constexpr char CHANNEL_POINT_COST[] = "cost";
constexpr char CHANNEL_POINT_TITLE[] = "title";
constexpr char CHANNEL_POINT_PROMPT[] = "prompt";
constexpr char CHANNEL_POINT_USER_INPUT_REQUIRED[] = "is_user_input_required";
constexpr char CHANNEL_POINT_MAX_PER_STREAM_SETTINGS[] = "max_per_stream_setting";
constexpr char CHANNEL_POINT_MAX_PER_STREAM[] = "max_per_stream";
constexpr char CHANNEL_POINT_MAX_PER_USER_PER_STREAM_SETTINGS[] = "max_per_user_per_stream_setting";
constexpr char CHANNEL_POINT_MAX_PER_USER_PER_STREAM[] = "max_per_user_per_stream";
constexpr char CHANNEL_POINT_GLOBAL_COOLDOWN_SETTINGS[] = "global_cooldown_setting";
constexpr char CHANNEL_POINT_GLOBAL_COOLDOWN_SECONDS[] = "global_cooldown_seconds";
constexpr char CHANNEL_POINT_PAUSED[] = "is_paused";
constexpr char CHANNEL_POINT_IN_STOCK[] = "is_in_stock";
constexpr char CHANNEL_POINT_COOLDOWN_EXPIRES[] = "cooldown_expires_at";

ChannelPointsReward::ChannelPointsReward(QObject* Parent)
    : QObject(Parent)
{

}

ChannelPointsReward::~ChannelPointsReward()
{
    LOG_INFO("Remove channel point reward %s", Title.toUtf8().data());
}

QJsonObject const ChannelPointsReward::GetNetworkData() const
{
    QJsonObject Data;

    Data[CHANNEL_POINT_TITLE] = Title;
    Data[CHANNEL_POINT_COST] = Cost;
    Data[CHANNEL_POINT_ENABLED] = bIsEnabled;

    if (bIsUserInputRequired) {
        Data[CHANNEL_POINT_USER_INPUT_REQUIRED] = bIsUserInputRequired;
        Data[CHANNEL_POINT_PROMPT] = UserPrompt;
    }

    return Data;
}

void ChannelPointsReward::ParseReply(QNetworkReply* Reply)
{
    if (Reply == nullptr) {
        return;
    }

    QJsonDocument InfoDocument = QJsonDocument::fromJson(Reply->readAll());
    if (InfoDocument.isEmpty()) {
        return;
    }

    QJsonArray Data = InfoDocument.toVariant().toJsonArray();
    if (!Data.isEmpty() && Data.count() == 1) {
        ParseJson(Data.at(0));
    }
}

void ChannelPointsReward::ParseJson(const QJsonValue& Value)
{
    if (!Value.isObject()) {
        return;
    }

    const QJsonObject Object = Value.toObject();
    if (!Object.contains(CHANNEL_POINT_ID)) {
        QJsonDocument LogDocument(Object);
        LOG_WARNING("ChannelPointsRewards: Invalid object = %s", LogDocument.toJson().data());
        return;
    }

    RewardID = Object[CHANNEL_POINT_ID].toString();
    Cost = Object[CHANNEL_POINT_COST].toInt();
    Title = Object[CHANNEL_POINT_TITLE].toString();
    UserPrompt = Object[CHANNEL_POINT_PROMPT].toString();
    bIsUserInputRequired = Object[CHANNEL_POINT_USER_INPUT_REQUIRED].toBool();
    bIsEnabled = Object[CHANNEL_POINT_ENABLED].toBool();

    LOG_INFO("ConfiguredNew ChannelPoints reward. {ID = %s, COST = %d, TITLE = %s, IsEnabled = %d}", RewardID.toUtf8().data(), Cost, Title.toUtf8().data(), bIsEnabled);
}
