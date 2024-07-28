#include "Managers/TwitchManager.h"
#include "Managers/TwitchNetworkAccessManager.h"
#include "System/Logger.h"
#include "Managers/NotificationsManager.h"
#include "Core/Twitch/ChannelPointsReward.h"
#include "Managers/NotificationsManager.h"

#include <QNetworkRequest>
#include <QNetworkReply>
#include <QDesktopServices>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>

TwitchManager::TwitchManager(QObject* Parent)
    :QObject(Parent)
{
}

TwitchManager::~TwitchManager()
{

}

void TwitchManager::SetNetworkManager(TwitchNetworkAccessManager* Manager)
{
    if (Manager == nullptr) {
        return;
    }

    if (NetworkManager) {
        QObject::disconnect(NetworkManager, &TwitchNetworkAccessManager::successfullyConnected, this, &TwitchManager::onConnectionSuccess);
        QObject::disconnect(NetworkManager, &TwitchNetworkAccessManager::failedToConnect, this, &TwitchManager::onConnectionFailed);
        QObject::disconnect(NetworkManager, &TwitchNetworkAccessManager::onChannelNameReceived, this, &TwitchManager::onChannelNameReceived);
    }

    NetworkManager = Manager;
    if (NetworkManager) {
        QObject::connect(NetworkManager, &TwitchNetworkAccessManager::successfullyConnected, this, &TwitchManager::onConnectionSuccess);
        QObject::connect(NetworkManager, &TwitchNetworkAccessManager::failedToConnect, this, &TwitchManager::onConnectionFailed);
        QObject::connect(NetworkManager, &TwitchNetworkAccessManager::onChannelNameReceived, this, &TwitchManager::onChannelNameReceived);
    }

}

bool TwitchManager::isAuthorized() const
{
    return bIsConnected;
}

void TwitchManager::requestChannelPointsRewards()
{
    if (NetworkManager == nullptr || !bIsConnected) {
        LOG_WARNING("Invalid Network manager, to request channel points rewards, or not connected to channel");
        return;
    }

    NetworkManager->Get("https://api.twitch.tv/helix/channel_points/custom_rewards", [this](const QJsonArray& Data) {
        ParseChannelPointsRewards(Data);
    });
}

void TwitchManager::createNewChannelPointsReward()
{
    if (QSharedPointer<ChannelPointsReward> NewReward = QSharedPointer<ChannelPointsReward>::create()) {
        NewReward->bIsEnabled = true;
        ChannelPointsRewards.push_back(NewReward);
        emit channelPointsRewardsUpdated();
    }

}

void TwitchManager::updateChannelPointsReward(ChannelPointsReward* RewardToUpdate)
{
    if (!RewardToUpdate) {
        LOG_WARNING("Invalid channel points reward to update");
        return;
    }

    if (RewardToUpdate->RewardID.isEmpty()) {
        CreateChannelPointsReward(QSharedPointer<ChannelPointsReward>(RewardToUpdate));
    } else {
        if (NetworkManager == nullptr || !bIsConnected) {
            LOG_WARNING("Invalid Network manager, to request channel points rewards, or not connected to channel");
            return;
        }

        NetworkManager->Patch("https://api.twitch.tv/helix/channel_points/custom_rewards", RewardToUpdate->GetNetworkData(), RewardToUpdate->RewardID, [this, RewardToUpdate](const QJsonArray &Data) {
            if (Data.count() == 1 && RewardToUpdate != nullptr) {
                RewardToUpdate->ParseJson(Data[0]);
                NotificationsManager::SendNotification("Twitch Manager", QString("Reward %1 was updated").arg(RewardToUpdate->Title));
                emit channelPointsRewardsUpdated();
            }
        });

    }
}

void TwitchManager::removeChannelPointsRewardById(const QString& ID)
{
    if (NetworkManager == nullptr || !bIsConnected) {
        LOG_WARNING("Invalid Network manager, to request channel points rewards, or not connected to channel");
        return;
    }

    NetworkManager->Delete("https://api.twitch.tv/helix/channel_points/custom_rewards", ID, [this](const QJsonArray &Data) {
        NotificationsManager::SendNotification("Twitch Manager", "Reward was removed");
        emit channelPointsRewardsUpdated();
    });
}


void TwitchManager::CreateChannelPointsReward(QSharedPointer<ChannelPointsReward> Reward)
{
    if (NetworkManager == nullptr || !bIsConnected) {
        LOG_WARNING("Invalid Network manager, to request channel points rewards, or not connected to channel");
        return;
    }

    NetworkManager->Post("https://api.twitch.tv/helix/channel_points/custom_rewards", Reward->GetNetworkData(), [this, Reward](const QJsonArray &Data) {
        if (Data.count() == 1 && Reward != nullptr) {
            Reward->ParseJson(Data[0]);
            emit channelPointsRewardsUpdated();
            NotificationsManager::SendNotification("Twitch Manager", QString("Custom reward %1, was successfully created!").arg(Reward->Title));
        }
    });
}

void TwitchManager::connectToTheChannel(const QString& Channel)
{
    if (NetworkManager && !Channel.isEmpty()) {
        ChannelName = Channel;
        NetworkManager->InitBroadcasterInfo(Channel);
    }
}

void TwitchManager::onConnectionSuccess()
{
    bIsConnected = true;
    emit connectionUpdated(bIsConnected);
}

void TwitchManager::onConnectionFailed()
{
    bIsConnected = false;
    emit connectionUpdated(bIsConnected);
}

void TwitchManager::authorize()
{
    if (NetworkManager == nullptr) {
        LOG_WARNING("TwitchManager: Network Manager not initialized");
        return;
    }

    emit authorizationURLReady(NetworkManager->GetAuthorizationURL());
}

void TwitchManager::userAuthorized(const QString& Token)
{
    if (!Token.isEmpty() && Token.length() == 30) {
        UserOAuthToken = Token;
        NotificationsManager::SendNotification("Twitch Manager", "Successfully authorized");

        if (NetworkManager) {
            NetworkManager->RequestChannelInfo(UserOAuthToken);
        }

    } else {
        NotificationsManager::SendNotification("Twitch Manager", "Authorization failed");
    }
}

void TwitchManager::ParseChannelPointsRewards(const QJsonArray& Rewards)
{
    if (Rewards.isEmpty()) {
        return;
    }

    ChannelPointsRewards.clear();

    for (const QJsonValue& Reward : Rewards) {
        if (QSharedPointer<ChannelPointsReward> NewReward = QSharedPointer<ChannelPointsReward>::create()) {
            NewReward->ParseJson(Reward);
            ChannelPointsRewards.push_back(NewReward);
        }
    }

    emit channelPointsRewardsUpdated();
}

void TwitchManager::onChannelNameReceived(const QString& NewChannelName)
{
    if (!NewChannelName.isEmpty()) {
        connectToTheChannel(NewChannelName);
    }
}

void TwitchManager::initByConfig(const ConfigObject* Config)
{
    if (!Config) {
        return;
    }

    const TwitchSettings Settings = Config->GetTwitchSettings();
    if (!isAuthorized() && !Settings.OAuthToken.isEmpty() && !Settings.ChannelName.isEmpty()) {
        userAuthorized(Settings.OAuthToken);
    }
}

void TwitchManager::saveConfig(ConfigObject* Config)
{
    if (Config) {
        Config->saveTwitchInfo(ChannelName, UserOAuthToken);
    }
}

int TwitchManager::getChannelPointsRewardsCount() const
{
    return ChannelPointsRewards.count();
}

QSharedPointer<ChannelPointsReward> TwitchManager::GetReward(int Index) const
{
    if (Index >= 0 && Index < ChannelPointsRewards.count()) {
        return ChannelPointsRewards[Index];
    }

    return nullptr;
}

ChannelPointsReward* TwitchManager::getChannelPointRewardByID(const QString& ID) const
{
    for (const QSharedPointer<ChannelPointsReward>& Reward : ChannelPointsRewards) {
        if (!Reward.isNull() && Reward->RewardID == ID) {
            return Reward.get();
        }
    }

    return nullptr;
}

bool TwitchManager::isCanCreateNewEmptyReward() const
{
    bool bCan = true;

    for (const QSharedPointer<ChannelPointsReward>& Reward : ChannelPointsRewards) {
        if (!Reward.isNull() && Reward->RewardID.isEmpty()) {
            bCan = false;
            break;
        }
    }

    return bCan;
}
