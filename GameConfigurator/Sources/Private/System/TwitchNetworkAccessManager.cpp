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
constexpr char TWITCH_BROADCASTER_ID_FIELD[] = "broadcasterId";

TwitchNetworkAccessManager::TwitchNetworkAccessManager(QObject* Parent)
    : QNetworkAccessManager(Parent)
{

}

void TwitchNetworkAccessManager::InitBroadcasterInfo(const QString& BroadcasterName)
{
    if (BroadcasterName.isEmpty())
    {
        NotificationsManager::SendNotification("Twitch Network Manager", "Invalid broadcaster name!\n User info cannot be initialized");
        return;
    }

    QNetworkRequest Request = CreateDefaultRequest("https://api.twitch.tv/helix/users?login=" + BroadcasterName.toLower());
    QNetworkReply* Reply = get(Request);
    QObject::connect(Reply, &QNetworkReply::finished, this, [=](){
        QByteArray Data = Reply->readAll();
        OnBroadcasterInfoReceived(Data);
    });
}

void TwitchNetworkAccessManager::OnBroadcasterInfoReceived(const QByteArray& Data)
{
    QJsonDocument InfoDocument = QJsonDocument::fromJson(Data);
    QVariantMap InfoMap = InfoDocument.toVariant().toMap();
    if (InfoMap.contains(TWITCH_DATA_FIELD))
    {
        QJsonArray DataArray = InfoMap[TWITCH_DATA_FIELD].toJsonArray();
        for (QJsonValueConstRef Item : DataArray)
        {
            QJsonObject ItemObj = Item.toObject();
            if (ItemObj.contains(TWITCH_ID_FIELD))
            {
                BroadcasterID = ItemObj[TWITCH_ID_FIELD].toString().toInt();
                LOG_INFO("BroadcasterID = %d", BroadcasterID);
                break;
            }
        }
    }
}

QNetworkRequest TwitchNetworkAccessManager::CreateDefaultRequest(const QString&& URL)
{
    QNetworkRequest Request(URL);
    QString AuthorizationKey("Bearer ");
    AuthorizationKey += QT_STRINGIFY(OAUTH_TOKEN);
    Request.setRawHeader("Authorization", AuthorizationKey.toUtf8());
    Request.setRawHeader("Client-Id", QT_STRINGIFY(CLIENT_ID));

    return Request;
}

QNetworkRequest TwitchNetworkAccessManager::CreateDefaultRequestWithBroadcasterID(const QString&& URL)
{
    QNetworkRequest Request(URL + QString("?broadcaster_id=%d").arg(BroadcasterID));
    QString AuthorizationKey("Bearer ");
    AuthorizationKey += QT_STRINGIFY(OAUTH_TOKEN);
    Request.setRawHeader("Authorization", AuthorizationKey.toUtf8());
    Request.setRawHeader("Client-Id", QT_STRINGIFY(CLIENT_ID));

    return Request;
}
