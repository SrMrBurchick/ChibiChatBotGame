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

    // Modifiers
    void InitBroadcasterInfo(const QString& BroadcasterName);

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
};
