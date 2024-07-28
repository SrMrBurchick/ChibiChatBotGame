#include "Models/Twitch/BaseTwitchModel.h"
#include "Managers/TwitchManager.h"
#include "System/Logger.h"

BaseTwitchModel::BaseTwitchModel(QObject* Parent)
    : QAbstractListModel(Parent)
{
}

BaseTwitchModel::~BaseTwitchModel()
{
    UnsubscribeFromTarget();
}

void BaseTwitchModel::subscribeOnTarget(TwitchManager* NewManager)
{
    if (NewManager)
    {
        UnsubscribeFromTarget();
        Manager = NewManager;

        if (Manager)
        {
            OnTargetSubscribed();
            LOG_INFO("Subscribed on target");
        }
    }
}

void BaseTwitchModel::OnTargetSubscribed()
{
    beginResetModel();
    endResetModel();
}

void BaseTwitchModel::UnsubscribeFromTarget()
{
    if (Manager)
    {
        Manager = nullptr;
    }
}
