#pragma once

#include <QObject>
#include <QSharedPointer>
#include <QMap>

#include "Configuration/ConfigObject.h"

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

    // Getters
    QSharedPointer<ChannelPointsReward> GetReward(int Index) const;


    // ================================ QML ====================================
    // Modifiers
    Q_INVOKABLE void connectToTheChannel(const QString& Channel);
    Q_INVOKABLE void initByConfig(const ConfigObject* Config);
    Q_INVOKABLE void saveConfig(ConfigObject* Config);

    // Rewards
    Q_INVOKABLE void createNewChannelPointsReward();
    Q_INVOKABLE void updateChannelPointsReward(ChannelPointsReward* RewardToUpdate);
    Q_INVOKABLE void removeChannelPointsRewardById(const QString& ID);
    Q_INVOKABLE ChannelPointsReward* getChannelPointRewardByID(const QString& ID) const;

    // Getter
    Q_INVOKABLE bool isAuthorized() const;
    Q_INVOKABLE int getChannelPointsRewardsCount() const;
    Q_INVOKABLE bool isCanCreateNewEmptyReward() const;

    // Event
    Q_INVOKABLE void requestChannelPointsRewards();
    Q_INVOKABLE void authorize();
    Q_INVOKABLE void userAuthorized(const QString& Token);


signals:
    void channelPointsRewardsUpdated();
    void connectionUpdated(bool isConnected);
    void authorizationURLReady(const QString& URL);

public slots:
    void onConnectionSuccess();
    void onConnectionFailed();
    void onChannelNameReceived(const QString& NewChannelName);

protected:
    void ParseChannelPointsRewards(const QJsonArray& Rewards);
    void CreateChannelPointsReward(QSharedPointer<ChannelPointsReward> Reward);

private:
    TwitchNetworkAccessManager* NetworkManager;
    QVector<QSharedPointer<ChannelPointsReward>> ChannelPointsRewards;
    bool bIsConnected = false;
    QString ChannelName;
    QString UserOAuthToken;
};
