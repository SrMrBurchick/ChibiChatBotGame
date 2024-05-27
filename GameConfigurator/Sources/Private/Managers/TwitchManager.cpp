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

