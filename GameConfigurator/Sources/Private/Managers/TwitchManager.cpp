#include "Managers/TwitchManager.h"

TwitchManager::TwitchManager(QObject* Parent)
    :QObject(Parent)
{
}

TwitchManager::~TwitchManager()
{

}

void TwitchManager::SetNetworkManager(TwitchNetworkAccessManager* Manager)
{
    // Stub
    // TODO: Unsub/Sub on manager
    NetworkManager = Manager;
}

bool TwitchManager::isConnectedToChannel() const
{
    // TODO: Check connection
    return true;
}

void TwitchManager::requestChannelPointsRewards()
{
    // TODO: https://dev.twitch.tv/docs/api/reference/#get-custom-reward
}

void TwitchManager::addNewChannelPointsReward(ChannelPointsReward* NewReward)
{
    // TODO:
}

void TwitchManager::removeChannelPointsRewardById(int Index)
{
    // TODO:
}
