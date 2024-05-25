#include "Managers/ActionsManager.h"
#include "Core/Action.h"
#include "System/Logger.h"

ActionsManager::ActionsManager(QObject* Parent)
    : QObject(Parent)
{
}

void ActionsManager::addNewAction(const QString& ActionName)
{
    if (GetActionByName(ActionName).isNull())
    {
        if (QSharedPointer<Action> NewAction = QSharedPointer<Action>::create())
        {
            NewAction->SetName(ActionName);
            Actions.push_back(NewAction);

            LOG_INFO("New Action added = %s", ActionName.toStdString().c_str());
            emit actionsUpdated();
        }
    }
}

void ActionsManager::removeActionById(int Index)
{
    if (Index >= 0 && Index <= Actions.count())
    {
        LOG_INFO("Remove action at = %d", Index);
        Actions.removeAt(Index);

        emit actionsUpdated();
    }
}

void ActionsManager::removeAction(QSharedPointer<Action> ActionToRemove)
{
    LOG_INFO("Remove action");
    Actions.removeOne(ActionToRemove);

    emit actionsUpdated();
}

QSharedPointer<Action> ActionsManager::GetActionByName(const QString& ActionName) const
{
    for (const QSharedPointer<Action>& TargetAction : Actions)
    {
        if (!TargetAction.isNull())
        {
            if (TargetAction->getName().toLower() == ActionName.toLower())
            {
                return TargetAction;
            }
        }
    }

    return nullptr;
}

QSharedPointer<Action> ActionsManager::GetActionById(int Index) const
{
    if (Index >= 0 && Index <= Actions.count())
    {
        return Actions[Index];
    }

    return nullptr;
}

Action* ActionsManager::getActionByName(const QString& ActionName) const
{
    return GetActionByName(ActionName).get();
}

Action* ActionsManager::getActionById(int Index) const
{
    return GetActionById(Index).get();
}

const QVector<QSharedPointer<Action>>& ActionsManager::getActions() const
{
    return Actions;
}

int ActionsManager::getActionsCount() const
{
    return Actions.count();
}

void ActionsManager::changeActionName(int Index, const QString& NewName)
{
    if (QSharedPointer<Action> TargetAction = GetActionById(Index))
    {
        TargetAction->SetName(NewName);
        emit actionsUpdated();
    }
}

void ActionsManager::markSelectedAction(int Index)
{
    if (QSharedPointer<Action> TargetAction = GetActionById(Index))
    {
        SelectedAction = TargetAction;
        emit actionSelected(TargetAction.get());
    }
}

bool ActionsManager::isGameDefaultAction(Action* ActionToCheck) const
{
    if (ActionToCheck != nullptr)
    {
        return GameDefaultActions.contains(ActionToCheck->getName());
    }

    return false;
}

bool ActionsManager::isTwitchDefaultAction(Action* ActionToCheck) const
{
    if (ActionToCheck != nullptr)
    {
        return TwitchDefaultActions.contains(ActionToCheck->getName());
    }

    return false;
}

bool ActionsManager::isDefaultAction(Action* ActionToCheck) const
{
    if (ActionToCheck != nullptr)
    {
        return isGameDefaultAction(ActionToCheck) || isTwitchDefaultAction(ActionToCheck);
    }

    return false;
}

bool ActionsManager::isDefaultAction(int Index) const
{
    return isDefaultAction(GetActionById(Index).get());
}

const QVector<QString>& ActionsManager::getGameDefaultActions() const
{
    return GameDefaultActions;
}

const QVector<QString>& ActionsManager::getTwitchDefaultAction() const
{
    return TwitchDefaultActions;
}

const QVector<QString> ActionsManager::getPossibleActionsToAdd() const
{
    QVector<QString> PossibleActions;
    PossibleActions.append(GameDefaultActions);
    PossibleActions.append(TwitchDefaultActions);

    PossibleActions.removeIf([=](const QString& name){
        return !GetActionByName(name).isNull();
    });

    return PossibleActions;
}

Action* ActionsManager::getSelectedAction() const
{
    return SelectedAction.get();
}
