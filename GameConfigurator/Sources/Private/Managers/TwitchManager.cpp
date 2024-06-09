#include "Managers/TwitchManager.h"
#include "System/TwitchNetworkAccessManager.h"
#include "System/Logger.h"
#include "Managers/NotificationsManager.h"

#include <QNetworkRequest>
#include <QNetworkReply>
#include <QDesktopServices>

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
    }

    NetworkManager = Manager;
    if (NetworkManager) {
        QObject::connect(NetworkManager, &TwitchNetworkAccessManager::successfullyConnected, this, &TwitchManager::onConnectionSuccess);
        QObject::connect(NetworkManager, &TwitchNetworkAccessManager::failedToConnect, this, &TwitchManager::onConnectionFailed);
    }

}

bool TwitchManager::isConnectedToChannel() const
{
    return bIsConnected;
}

void TwitchManager::requestChannelPointsRewards()
{
    if (NetworkManager == nullptr || !bIsConnected) {
        LOG_WARNING("Invalid Network manager, to request channel points rewards, or not connected to channel");
        return;
    }

    LOG_INFO("Request ChannelPoints Rewards");
    NetworkManager->Get("https://api.twitch.tv/helix/channel_points/custom_rewards", [](const QByteArray& Data) {
        LOG_INFO("Received = %s", Data.data());
    });
}

void TwitchManager::addNewChannelPointsReward(ChannelPointsReward* NewReward)
{
    // TODO: move shared pointer to network manager
}

void TwitchManager::removeChannelPointsRewardById(int Index)
{
    // TODO:
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
    requestChannelPointsRewards();
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
    } else {
        NotificationsManager::SendNotification("Twitch Manager", "Authorization failed");
    }
}
