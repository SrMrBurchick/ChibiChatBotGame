#include "Managers/ActionsManager.h"
#include "Core/Action.h"
#include "System/Logger.h"

#include <QSharedPointer>
#include <QQmlEngine>
#include <QJsonArray>
#include <QJsonObject>

ActionsManager::ActionsManager(QObject* Parent)
    : QObject(Parent)
{
    SelectedAction = nullptr;
}

QSharedPointer<Action> ActionsManager::CreateNewAction(const QString& ActionName)
{
    if (GetActionByName(ActionName).isNull())
    {
        if (QSharedPointer<Action> NewAction = QSharedPointer<Action>::create())
        {
            NewAction->SetName(ActionName);
            Actions.push_back(NewAction);
            if (ActionConfig* actionConfig = NewAction->getConfig()) {
                if (isTwitchDefaultAction(NewAction.get())) {
                    actionConfig->EventType = TwitchDefaultActions[ActionName];
                } else {
                    actionConfig->EventType = "channel.channel_points_custom_reward_redemption.add";
                }
            }

            LOG_INFO("New Action added = %s", ActionName.toStdString().c_str());
            return NewAction;
        }
    }

    return nullptr;
}

void ActionsManager::addNewAction(const QString& ActionName)
{
    if (!CreateNewAction(ActionName).isNull())
    {
        emit actionsUpdated();
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
    QSharedPointer<Action> TargetAction = GetActionByName(ActionName);
    if (!TargetAction.isNull()) {
        QQmlEngine::setObjectOwnership(TargetAction.data(), QQmlEngine::CppOwnership);
    }
    return TargetAction.data();
}

Action* ActionsManager::getActionById(int Index) const
{
    QSharedPointer<Action> TargetAction = GetActionById(Index);
    if (!TargetAction.isNull()) {
        QQmlEngine::setObjectOwnership(TargetAction.data(), QQmlEngine::CppOwnership);
    }
    return TargetAction.data();
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
        if (!TargetAction.isNull()) {
            QQmlEngine::setObjectOwnership(TargetAction.data(), QQmlEngine::CppOwnership);
        }

        emit actionSelected(TargetAction.data());
        SelectedAction = TargetAction;
        if (SelectedAction)
        {
            SelectedAction->MarkSelected();
        }
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

const QVector<QString> ActionsManager::getTwitchDefaultAction() const
{
    return TwitchDefaultActions.keys().toVector();
}

const QVector<QString> ActionsManager::getPossiblePredefinedActionsToAdd() const
{
    QVector<QString> PossibleActions;
    for (QSharedPointer<Action> Item : Actions) {
        if (!isTwitchDefaultAction(Item.get())) {
            PossibleActions.append(Item->getName());
        }
    }

    return PossibleActions;
}


const QVector<QString> ActionsManager::getPossibleActionsToAdd() const
{
    QVector<QString> PossibleActions;
    PossibleActions.append(GameDefaultActions);
    PossibleActions.append(getTwitchDefaultAction());

    PossibleActions.removeIf([this](const QString& name){
        return !GetActionByName(name).isNull();
    });

    return PossibleActions;
}

Action* ActionsManager::getSelectedAction() const
{
    if (!SelectedAction.isNull()) {
        QQmlEngine::setObjectOwnership(SelectedAction.data(), QQmlEngine::CppOwnership);
    }

    return SelectedAction.data();
}

void ActionsManager::saveConfig(ConfigObject* Config)
{
    if (!Config) {
        LOG_WARNING("Actions Manager: Invalid config to save");
        return;
    }

    Config->SaveActions(Actions);
    Config->saveConfig();
}

void ActionsManager::initByConfig(const ConfigObject* Config)
{
    if (!Config) {
        LOG_WARNING("Actions Manager: Invalid config to initialize");
        return;
    }

    Config->InitActionsManager(this);
}
