#include "Managers/ActionsManager.h"
#include "Core/Action.h"
#include "System/Logger.h"

ActionsManager::ActionsManager(QObject* Parent)
    : QObject(Parent)
{
}

void ActionsManager::addNewAction(const QString& ActionName)
{
    if (!getActionByName(ActionName))
    {
        if (QSharedPointer<Action> NewAction = QSharedPointer<Action>::create())
        {
            NewAction->SetName(ActionName);
            Actions.push_back(NewAction);

            emit actionsUpdated();
        }
    }
}

void ActionsManager::removeActionById(int Index)
{
    if (Index > 0 && Index <= Actions.count())
    {
        Actions.removeAt(Index);

        emit actionsUpdated();
    }
}

void ActionsManager::removeAction(QSharedPointer<Action> ActionToRemove)
{
    Actions.removeOne(ActionToRemove);

    emit actionsUpdated();
}

QSharedPointer<Action> ActionsManager::getActionByName(const QString& ActionName) const
{
    for (const QSharedPointer<Action>& Action : Actions)
    {
        if (!Action.isNull())
        {
            if (Action->getName() == ActionName)
            {
                return Action;
            }
        }
    }

    return nullptr;
}

QSharedPointer<Action> ActionsManager::getActionById(int Index) const
{
    if (Index >= 0 && Index <= Actions.count())
    {
        return Actions[Index];
    }

    return nullptr;
}

const QVector<QSharedPointer<Action>>& ActionsManager::getActions() const
{
    return Actions;
}

int ActionsManager::getActionsCount() const
{
    return Actions.count();
}
