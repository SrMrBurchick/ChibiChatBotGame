#pragma once

#include "qcontainerfwd.h"
#include <QNetworkAccessManager>
#include <functional>

class QNetworkRequest;

class TwitchNetworkAccessManager : public QNetworkAccessManager
{
    Q_OBJECT

public:
    explicit TwitchNetworkAccessManager(QObject* Parent = nullptr);
    virtual ~TwitchNetworkAccessManager() = default;

    // ============================== C++ ======================================
    // Getters
    const QString GetAuthorizationURL() const;

    // Modifiers
    void InitBroadcasterInfo(const QString& BroadcasterName);
    void SetupRedirectURI(const QString& URI);
    void RequestChannelInfo(const QString& OAuthToken);

    // Events
    void Get(const QString URL, std::function<void(const QJsonArray& Data)> Handler);
    void Post(const QString URL, std::function<void(const QJsonArray& Data)> Handler);
    void Delete(const QString URL, std::function<void(const QJsonArray& Data)> Handler);


signals:
    void successfullyConnected();
    void failedToConnect();
    void dataReceived(const QByteArray& Data);
    void onChannelNameReceived(const QString& ChannelName);

protected:
    void OnBroadcasterInfoReceived(const QByteArray& Data);
    QNetworkRequest CreateDefaultRequest(const QString& URL);
    QNetworkRequest CreateDefaultRequestWithBroadcasterID(const QString& URL);
    QString GetOAuthToken() const;

private:
    QString BroadcasterID;
    QString RedirectURI;
    QString UserToken;
};
