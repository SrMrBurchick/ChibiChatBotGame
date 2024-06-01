#pragma once

#include <QNetworkAccessManager>

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

protected:
    void OnBroadcasterInfoReceived(const QByteArray& Data);
    QNetworkRequest CreateDefaultRequest(const QString&& URL);
    QNetworkRequest CreateDefaultRequestWithBroadcasterID(const QString&& URL);

private:
    int BroadcasterID = 0;
};
