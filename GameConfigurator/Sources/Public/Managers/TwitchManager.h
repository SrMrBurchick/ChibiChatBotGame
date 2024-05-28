#pragma once

#include <QObject>
#include <QSharedPointer>

class TwitchNetworkAccessManager;

class ChannelPointsReward;

class TwitchManager : public QObject
{
    Q_OBJECT

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

    // Getter
    Q_INVOKABLE bool isConnectedToChannel() const;

    // Event
    Q_INVOKABLE void requestChannelPointsRewards();

signals:
    void channelPointsRewardsUpdated();

private:
    TwitchNetworkAccessManager* NetworkManager;
    QVector<QSharedPointer<ChannelPointsReward>> ChannelPointsRewards;
};
