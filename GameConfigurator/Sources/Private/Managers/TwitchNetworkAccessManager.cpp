#include "Managers/TwitchNetworkAccessManager.h"

#include "Managers/NotificationsManager.h"
#include "System/Logger.h"

#include <QNetworkReply>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QVariantMap>
#include <QUrlQuery>

constexpr char TWITCH_DATA_FIELD[] = "data";
constexpr char TWITCH_ID_FIELD[] = "id";
constexpr char TWITCH_ERROR_FIELD[] = "error";
constexpr char TWITCH_MESSAGE_FIELD[] = "message";
constexpr char TWITCH_BROADCASTER_ID_FIELD[] = "broadcasterId";
constexpr char TWITCH_LOGIN_FIELD[] = "login";


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
    if (QNetworkReply* Reply = get(Request)) {
        QObject::connect(Reply, &QNetworkReply::finished, this, [Reply, this](){
            OnBroadcasterInfoReceived(Reply->readAll());
            emit responseState(Reply->error() == QNetworkReply::NoError);
        });
    } else {
        emit responseState(false);
    }
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
                BroadcasterID = ItemObj[TWITCH_ID_FIELD].toString();
                LOG_INFO("BroadcasterID = %s", BroadcasterID.toUtf8().data());
                if (!BroadcasterID.isEmpty()) {
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
    QUrl RequestURL(URL);
    QNetworkRequest Request(RequestURL);
    Request.setRawHeader("Authorization", GetOAuthToken().toUtf8());
    Request.setRawHeader("Client-Id", QT_STRINGIFY(CLIENT_ID));

    return Request;
}

QNetworkRequest TwitchNetworkAccessManager::CreateDefaultRequestWithBroadcasterID(const QString& URL)
{
    QUrl RequestURL(URL);
    QUrlQuery Query;
    Query.addQueryItem("broadcaster_id", BroadcasterID);
    RequestURL.setQuery(Query);
    QNetworkRequest Request(RequestURL);
    Request.setRawHeader("Authorization", GetOAuthToken().toUtf8());
    Request.setRawHeader("Client-Id", QT_STRINGIFY(CLIENT_ID));

    return Request;
}

void TwitchNetworkAccessManager::Get(const QString URL, std::function<void(const QJsonArray& Data)> Handler)
{
    QNetworkRequest Request = CreateDefaultRequestWithBroadcasterID(URL);
    if (QNetworkReply* Reply = get(Request)) {
        QObject::connect(Reply, &QNetworkReply::finished, this, [Handler, Reply, this](){
            QByteArray Data = Reply->readAll();
            QJsonDocument InfoDocument = QJsonDocument::fromJson(Data);
            QJsonObject InfoMap = InfoDocument.object();
            if (InfoMap.contains(TWITCH_ERROR_FIELD)) {
                LOG_CRITICAL("TwitchNetworkAccessManager", "Failed to get data. Reason = %s", Data.data());
            } else {
                if (InfoMap.contains(TWITCH_DATA_FIELD)) {
                    Handler(InfoMap[TWITCH_DATA_FIELD].toArray());
                }
            }

            emit responseState(Reply->error() == QNetworkReply::NoError);
        });
    } else {
        LOG_CRITICAL("Failed to create reply for request: %s", URL.toStdString().c_str());
        NotificationsManager::SendNotification("TwitchNetworkAccessManager", "Failed to get reply");
        emit responseState(false);
    }
}

void TwitchNetworkAccessManager::Post(const QString URL, const QJsonObject& Data,  std::function<void(const QJsonArray& Data)> Handler)
{
    QNetworkRequest Request = CreateDefaultRequestWithBroadcasterID(URL);
    Request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    if (QNetworkReply* Reply = post(Request, QJsonDocument(Data).toJson())) {
        QObject::connect(Reply, &QNetworkReply::finished, this, [Handler, Reply, this](){
            QByteArray Data = Reply->readAll();
            QJsonDocument InfoDocument = QJsonDocument::fromJson(Data);
            QJsonObject InfoMap = InfoDocument.object();
            if (InfoMap.contains(TWITCH_ERROR_FIELD)) {
                LOG_CRITICAL("TwitchNetworkAccessManager", "Failed to get data. Reason = %s", Data.data());
            } else {
                if (InfoMap.contains(TWITCH_DATA_FIELD)) {
                    Handler(InfoMap[TWITCH_DATA_FIELD].toArray());
                }
            }

            emit responseState(Reply->error() == QNetworkReply::NoError);
        });
    } else {
        LOG_CRITICAL("Failed to create reply for request: %s", URL.toStdString().c_str());
        NotificationsManager::SendNotification("TwitchNetworkAccessManager", "Failed to get reply");
        emit responseState(false);
    }
}

void TwitchNetworkAccessManager::Delete(const QString URL, const QString& ID, std::function<void(const QJsonArray& Data)> Handler)
{
    QNetworkRequest Request = CreateDefaultRequestWithBroadcasterID(URL);
    QUrl RequestURL(Request.url());
    QUrlQuery Query;
    Query.addQueryItem(TWITCH_ID_FIELD, ID);
    Query.addQueryItem("broadcaster_id", BroadcasterID);
    RequestURL.setQuery(Query);
    Request.setUrl(RequestURL);

    if (QNetworkReply* Reply = deleteResource(Request)) {
        QObject::connect(Reply, &QNetworkReply::finished, this, [Handler, Reply, this](){
            QByteArray Data = Reply->readAll();
            LOG_INFO("Delete result: %s", Data.data());
            QJsonArray Array;
            Handler(Array);
            emit responseState(Reply->error() == QNetworkReply::NoError);
        });
    } else {
        LOG_CRITICAL("Failed to create reply for request: %s", URL.toStdString().c_str());
        NotificationsManager::SendNotification("TwitchNetworkAccessManager", "Failed to get reply");
        emit responseState(false);
    }
}

void TwitchNetworkAccessManager::Patch(const QString URL, const QJsonObject& Data, const QString& ID, std::function<void(const QJsonArray& Data)> Handler)
{
    QNetworkRequest Request = CreateDefaultRequestWithBroadcasterID(URL);
    QUrl RequestURL(Request.url());
    QUrlQuery Query;
    Query.addQueryItem(TWITCH_ID_FIELD, ID);
    Query.addQueryItem("broadcaster_id", BroadcasterID);
    RequestURL.setQuery(Query);
    Request.setUrl(RequestURL);
    Request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    if (QNetworkReply* Reply = sendCustomRequest(Request, "PATCH", QJsonDocument(Data).toJson())) {
        QObject::connect(Reply, &QNetworkReply::finished, this, [Handler, Reply, this](){
            QByteArray Data = Reply->readAll();
            QJsonDocument InfoDocument = QJsonDocument::fromJson(Data);
            QJsonObject InfoMap = InfoDocument.object();
            if (InfoMap.contains(TWITCH_ERROR_FIELD)) {
                LOG_CRITICAL("TwitchNetworkAccessManager", "Failed to get data. Reason = %s", Data.data());
            } else {
                if (InfoMap.contains(TWITCH_DATA_FIELD)) {
                    Handler(InfoMap[TWITCH_DATA_FIELD].toArray());
                }
            }

            emit responseState(Reply->error() == QNetworkReply::NoError);
        });
    } else {
        LOG_CRITICAL("Failed to create reply for request: %s", URL.toStdString().c_str());
        NotificationsManager::SendNotification("TwitchNetworkAccessManager", "Failed to get reply");
        emit responseState(false);
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

void TwitchNetworkAccessManager::RequestChannelInfo(const QString& OAuthToken)
{
    QNetworkRequest Request(QUrl("https://api.twitch.tv/helix/users"));
    UserToken = OAuthToken;
    Request.setRawHeader("Authorization", GetOAuthToken().toUtf8());
    Request.setRawHeader("Client-Id", QT_STRINGIFY(CLIENT_ID));

    if (QNetworkReply* Reply = get(Request)) {
        QObject::connect(Reply, &QNetworkReply::finished, [Reply, this]() {
            if (Reply->error() == QNetworkReply::NoError) {
                QByteArray response = Reply->readAll();
                LOG_INFO("Channel info = %s", response.data());
                QJsonDocument jsonDoc = QJsonDocument::fromJson(response);
                QJsonObject jsonObj = jsonDoc.object();
                QJsonArray DataArray = jsonObj[TWITCH_DATA_FIELD].toArray();
                for (QJsonValueConstRef Item : DataArray) {
                    QJsonObject ItemObj = Item.toObject();
                    if (ItemObj.contains(TWITCH_LOGIN_FIELD)) {
                        emit onChannelNameReceived(ItemObj[TWITCH_LOGIN_FIELD].toString());
                        emit responseState(Reply->error() == QNetworkReply::NoError);
                        return;
                    }
                }

                emit responseState(Reply->error() == QNetworkReply::NoError);

            } else {
                LOG_CRITICAL("Request channel info error! %s", Reply->errorString().toStdString().c_str());
                emit responseState(false);
            }
        });
    }
}

QString TwitchNetworkAccessManager::GetOAuthToken() const
{
    return QString("Bearer ") + (UserToken.isEmpty() ? QT_STRINGIFY(OAUTH_TOKEN) : UserToken);
}
