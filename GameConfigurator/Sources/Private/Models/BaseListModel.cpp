#include "Models/BaseListModel.h"
#include "Managers/ActionsManager.h"

BaseListModel::BaseListModel(QObject* Parent)
    : QAbstractListModel(Parent)
{
}

void BaseListModel::subscribeOnTarget(ActionsManager* NewManager)
{
    if (NewManager)
    {
        if (Manager)
        {
            Manager->disconnect();
        }

        Manager = NewManager;
        QObject::connect(Manager, &ActionsManager::actionsUpdated, [=](){
            OnActionsUpdated();
        });

        OnTargetSubscribed();
    }
}
