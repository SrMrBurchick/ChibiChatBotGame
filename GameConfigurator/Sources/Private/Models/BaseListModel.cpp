#include "Models/BaseListModel.h"
#include "Managers/ActionsManager.h"
#include "System/Logger.h"

BaseListModel::BaseListModel(QObject* Parent)
    : QAbstractListModel(Parent)
{
}

BaseListModel::~BaseListModel()
{
    UnsubscribeFromTarget();
}

void BaseListModel::subscribeOnTarget(ActionsManager* NewManager)
{
    if (NewManager)
    {
        UnsubscribeFromTarget();
        Manager = NewManager;

        if (Manager)
        {
            QObject::connect(Manager, &ActionsManager::actionsUpdated, this, &BaseListModel::onActionsUpdated);

            OnTargetSubscribed();
            LOG_INFO("Subscribed on target");
        }
    }
}

void BaseListModel::OnActionsUpdated()
{
    beginResetModel();
    endResetModel();
}

void BaseListModel::OnTargetSubscribed()
{
    beginResetModel();
    endResetModel();
}

void BaseListModel::onActionsUpdated()
{
    OnActionsUpdated();
}

void BaseListModel::UnsubscribeFromTarget()
{
    if (Manager)
    {
        QObject::disconnect(Manager, &ActionsManager::actionsUpdated, this, &BaseListModel::onActionsUpdated);
    }
}
