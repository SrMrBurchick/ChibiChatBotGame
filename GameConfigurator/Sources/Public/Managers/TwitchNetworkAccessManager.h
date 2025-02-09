#pragma once

#include "qcontainerfwd.h"
#include <QNetworkAccessManager>
#include <functional>


class QNetworkRequest;
class TwitchManager;
class QOAuth2AuthorizationCodeFlow;

class TwitchNetworkAccessManager : public QNetworkAccessManager
{
    Q_OBJECT

public:
    explicit TwitchNetworkAccessManager(QObject* Parent = nullptr);
    virtual ~TwitchNetworkAccessManager() = default;

    // ============================== C++ ======================================
    // Getters
    const QString GetAuthorizationURL() const;
    const QString GetTokenRequestURL(const QString& AuthCode);
    static QString ParseToken(const QString& Data);

    // Modifiers
    void InitBroadcasterInfo(const QString& BroadcasterName);
    void SetupRedirectURI(const QString& URI);
    void RequestChannelInfo(const QString& OAuthToken);
    void SetupAuthorizationFlow(QOAuth2AuthorizationCodeFlow& Flow);

    // Events
    void Get(const QString URL, std::function<void(const QJsonArray& Data)> Handler);
    void Post(const QString URL, const QJsonObject& Data, std::function<void(const QJsonArray& Data)> Handler);
    void Delete(const QString URL, const QString& ID, std::function<void(const QJsonArray& Data)> Handler);
    void Patch(const QString URL, const QJsonObject& Data, const QString& ID, std::function<void(const QJsonArray& Data)> Handler);
    void RawPost(const QString URL, const QJsonObject& Data, std::function<void(const QJsonObject& Data)> Handler);


signals:
    void successfullyConnected();
    void failedToConnect();
    void dataReceived(const QByteArray& Data);
    void onChannelNameReceived(const QString& ChannelName);
    void responseState(bool isSuccess);

protected:
    void OnBroadcasterInfoReceived(const QByteArray& Data);
    QNetworkRequest CreateDefaultRequest(const QString& URL);
    QNetworkRequest CreateDefaultRequestWithBroadcasterID(const QString& URL);
    QString GetOAuthToken() const;

private:
    QString BroadcasterID;
    QString RedirectURI;
    QString UserToken;

    friend TwitchManager;
};
