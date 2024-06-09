#pragma once

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

    // Events
    void Get(const QString& URL, std::function<void(const QByteArray& Data)> Handler);


signals:
    void successfullyConnected();
    void failedToConnect();
    void dataReceived(const QByteArray& Data);

protected:
    void OnBroadcasterInfoReceived(const QByteArray& Data);
    QNetworkRequest CreateDefaultRequest(const QString& URL);
    QNetworkRequest CreateDefaultRequestWithBroadcasterID(const QString& URL);

private:
    int BroadcasterID = 0;
    QString RedirectURI;
};
