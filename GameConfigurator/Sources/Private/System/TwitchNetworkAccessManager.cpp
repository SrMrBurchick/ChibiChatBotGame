#include "System/TwitchNetworkAccessManager.h"

#include "Managers/NotificationsManager.h"
#include "System/Logger.h"

#include <QNetworkReply>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QVariantMap>

constexpr char TWITCH_DATA_FIELD[] = "data";
constexpr char TWITCH_ID_FIELD[] = "id";
constexpr char TWITCH_ERROR_FIELD[] = "error";
constexpr char TWITCH_MESSAGE_FIELD[] = "message";
constexpr char TWITCH_BROADCASTER_ID_FIELD[] = "broadcasterId";

// Data
constexpr char TWITCH_ACCESS_SCOPES[] = "channel:read:redemptions channel:manage:redemptions";

TwitchNetworkAccessManager::TwitchNetworkAccessManager(QObject* Parent)
    : QNetworkAccessManager(Parent)
{

}

void TwitchNetworkAccessManager::InitBroadcasterInfo(const QString& BroadcasterName)
{
    if (BroadcasterName.isEmpty()) {
        NotificationsManager::SendNotification("Twitch Network Manager", "Invalid broadcaster name!\n User info cannot be initialized");
        return;
    }

    QNetworkRequest Request = CreateDefaultRequest("https://api.twitch.tv/helix/users?login=" + BroadcasterName.toLower());
    QNetworkReply* Reply = get(Request);
    QObject::connect(Reply, &QNetworkReply::finished, this, [=](){
        OnBroadcasterInfoReceived(Reply->readAll());
    });
}

void TwitchNetworkAccessManager::OnBroadcasterInfoReceived(const QByteArray& Data)
{
    QJsonDocument InfoDocument = QJsonDocument::fromJson(Data);
    QVariantMap InfoMap = InfoDocument.toVariant().toMap();
    if (InfoMap.contains(TWITCH_DATA_FIELD)) {
        QJsonArray DataArray = InfoMap[TWITCH_DATA_FIELD].toJsonArray();
        for (QJsonValueConstRef Item : DataArray) {
            QJsonObject ItemObj = Item.toObject();
            if (ItemObj.contains(TWITCH_ID_FIELD)) {
                BroadcasterID = ItemObj[TWITCH_ID_FIELD].toString().toInt();
                LOG_INFO("BroadcasterID = %d", BroadcasterID);
                if (BroadcasterID > 0) {
                    NotificationsManager::SendNotification("Twitch Network Manager", "Successfully connected to chanel!");
                    emit successfullyConnected();
                }

                break;
            }
        }
    } else if (InfoMap.contains(TWITCH_ERROR_FIELD)) {
        NotificationsManager::SendNotification("Twitch Network Manager", QString("Failed to connect! %1").arg(InfoMap[TWITCH_MESSAGE_FIELD].toByteArray().data()));
        LOG_CRITICAL("TwitchNetworkManager: [ConnectionError]: %s", Data.data());
        emit failedToConnect();
    }
}

QNetworkRequest TwitchNetworkAccessManager::CreateDefaultRequest(const QString& URL)
{
    QNetworkRequest Request(URL);
    QString AuthorizationKey("Bearer ");
    AuthorizationKey += QT_STRINGIFY(OAUTH_TOKEN);
    Request.setRawHeader("Authorization", AuthorizationKey.toUtf8());
    Request.setRawHeader("Client-Id", QT_STRINGIFY(CLIENT_ID));

    return Request;
}

QNetworkRequest TwitchNetworkAccessManager::CreateDefaultRequestWithBroadcasterID(const QString& URL)
{
    QNetworkRequest Request(URL + QString("?broadcaster_id=%1").arg(BroadcasterID));
    QString AuthorizationKey("Bearer ");
    AuthorizationKey += QT_STRINGIFY(OAUTH_TOKEN);
    Request.setRawHeader("Authorization", AuthorizationKey.toUtf8());
    Request.setRawHeader("Client-Id", QT_STRINGIFY(CLIENT_ID));

    return Request;
}

void TwitchNetworkAccessManager::Get(const QString& URL, std::function<void(const QByteArray& Data)> Handler)
{
    QNetworkRequest Request = CreateDefaultRequestWithBroadcasterID(URL);
    if (QNetworkReply* Reply = get(Request)) {
        QObject::connect(Reply, &QNetworkReply::finished, this, [Handler, Reply](){
            QByteArray Data = Reply->readAll();
            QJsonDocument InfoDocument = QJsonDocument::fromJson(Data);
            QVariantMap InfoMap = InfoDocument.toVariant().toMap();
            if (InfoMap.contains(TWITCH_ERROR_FIELD)) {
                LOG_CRITICAL("TwitchNetworkAccessManager", "Failed to get data. Reason = %s", Data.data());
            } else {
                Handler(Data);
            }
        });
    } else {
        LOG_CRITICAL("Failed to create reply for request: %s", URL.toStdString().c_str());
        NotificationsManager::SendNotification("TwitchNetworkAccessManager", "Failed to get reply");
    }
}

const QString TwitchNetworkAccessManager::GetAuthorizationURL() const
{
    return QString(
        "https://id.twitch.tv/oauth2/authorize"
        "?client_id=%1"
        "&redirect_uri=https://localhost:1337"
        "&response_type=token"
        "&force_verify=true"
        "&scope=channel:read:redemptions channel:manage:redemptions"
        "&state=unique_state")
        .arg(QT_STRINGIFY(CLIENT_ID));
}

void TwitchNetworkAccessManager::SetupRedirectURI(const QString& URI)
{
    if (!URI.isEmpty()) {
        RedirectURI = URI;
    }
}
