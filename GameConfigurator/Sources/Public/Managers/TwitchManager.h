#pragma once

#include <QObject>
#include <QSharedPointer>

class TwitchNetworkAccessManager;

class ChannelPointsReward;

class TwitchManager : public QObject
{
    Q_OBJECT

    Q_PROPERTY(bool isConnected MEMBER bIsConnected NOTIFY connectionUpdated)

public:
    explicit TwitchManager(QObject* Parent = nullptr);
    virtual ~TwitchManager();

    // ================================ C++ ====================================
    // Modifiers
    void SetNetworkManager(TwitchNetworkAccessManager* Manager);

    // ================================ QML ====================================
    // Modifiers
    Q_INVOKABLE void addNewChannelPointsReward(ChannelPointsReward* NewReward);
    Q_INVOKABLE void removeChannelPointsRewardById(int Index);
    Q_INVOKABLE void connectToTheChannel(const QString& Channel);

    // Getter
    Q_INVOKABLE bool isConnectedToChannel() const;

    // Event
    Q_INVOKABLE void requestChannelPointsRewards();

signals:
    void channelPointsRewardsUpdated();
    void connectionUpdated(bool isConnected);

public slots:
    void onConnectionSuccess();
    void onConnectionFailed();

private:
    TwitchNetworkAccessManager* NetworkManager;
    QVector<QSharedPointer<ChannelPointsReward>> ChannelPointsRewards;
    bool bIsConnected = false;
    QString ChannelName = "";
};
