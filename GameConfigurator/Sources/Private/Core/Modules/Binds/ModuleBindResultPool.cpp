#include "Core/Modules/Binds/ModuleBindResultPool.h"
#include "Core/Modules/Binds/ModuleBindResult.h"

#include "System/Logger.h"
#include "System/AccessPoint.h"

#include "Managers/ModulesManger.h"
#include "Managers/ActionsManager.h"
#include "Managers/NotificationsManager.h"
#include "Core/Action.h"
#include "Core/Modules/Module.h"

#include <QQmlEngine>
#include <QJsonObject>
#include <QJsonArray>

constexpr char POSTFIX [] = "postfix";
constexpr char RESULTS [] = "results";

CBModuleBindResultPool::CBModuleBindResultPool(QObject* Parent)
    : QObject(Parent)
{

}

CBModuleBindResultPool::~CBModuleBindResultPool()
{

}

QSharedPointer<CBModuleBindResultPool> CBModuleBindResultPool::CreatePool(const QString& Postfix)
{
    if (QSharedPointer<CBModuleBindResultPool> NewPool = QSharedPointer<CBModuleBindResultPool>::create()) {
        NewPool->PoolPostfix = Postfix;
        return NewPool;
    }

    return nullptr;
}

int CBModuleBindResultPool::getResultsCount() const
{
    return Results.count();
}

CBModuleBindResult* CBModuleBindResultPool::getResult(int Index) const
{
    if (Index < 0 || Index >= Results.count()) {
        return nullptr;
    }

    if (QSharedPointer<CBModuleBindResult> Result = Results[Index]) {
        QQmlEngine::setObjectOwnership(Result.data(), QQmlEngine::CppOwnership);
        return Result.data();
    }

    return nullptr;
}

const QString& CBModuleBindResultPool::getPoolPostfix() const
{
    return PoolPostfix;
}

void CBModuleBindResultPool::RemoveBindByAction(const QWeakPointer<Action> ActionToRemove)
{
    QSharedPointer<CBModuleBindResult> BindResult = nullptr;
    for (const QSharedPointer<CBModuleBindResult>& Result : Results) {
        if (Result->IsBindedTo(ActionToRemove)) {
            BindResult = Result;
            break;
        }
    }

    if (!BindResult.isNull()) {
        Results.removeOne(BindResult);
    }
}

void CBModuleBindResultPool::bindNewAction(ActionsManager* Manager, CBModulesManager* ModulesManager, const QString& ActionName)
{
    if (!Manager || !ModulesManager) {
        LOG_WARNING("Invalid params {ActionManager = %p, ModulesManger = %p}", Manager, ModulesManager);
        return;
    }

    if (CBModule* Module = ModulesManager->getSelectedModule()) {
        if (QSharedPointer<Action> NewAction = Manager->GetActionByName(ActionName)) {
            if (!Module->HasBindedAction(NewAction)) {
                if (CanBindAction(NewAction)) {
                    if (QSharedPointer<CBModuleBindResult> NewResult = CBModuleBindResult::CreateResult(NewAction)) {
                        Results.push_back(NewResult);

                        QWeakPointer<Action> ActionToRemoveWeak = NewAction.toWeakRef();
                        QObject::connect(NewAction.data(), &Action::beginRemove, [this, ActionToRemoveWeak](){
                            RemoveBindByAction(ActionToRemoveWeak);
                        });

                        emit bindsUpdated();
                    }
                }
            } else {
                NotificationsManager::SendNotification("Module pool", "You cannot bind action that already listened");
                return;
            }
        }
    }
}

bool CBModuleBindResultPool::CanBindAction(const QSharedPointer<Action>& TargetAction) const
{
    for (const QSharedPointer<CBModuleBindResult>& Result : Results) {
        if (Result->IsBindedTo(TargetAction)) {
            return false;
        }
    }

    return true;
}

void CBModuleBindResultPool::removeResult(int Index)
{
    if (Index < 0 || Index >= Results.count()) {
        return;
    }

    Results.remove(Index);
    emit bindsUpdated();
}

void CBModuleBindResultPool::setNewPosition(int Position)
{
    LOG_INFO("Try to set new position %d", Position);
    if (Position >= 0) {
        NewPosition = Position;
    }
}

void CBModuleBindResultPool::commitSwap(int OldPosition)
{
    ChangeResultPosition(OldPosition);
}

void CBModuleBindResultPool::ChangeResultPosition(int OldPosition)
{
    LOG_INFO("Try to swap items: %d to %d", OldPosition, NewPosition);
    if (OldPosition < 0 || OldPosition >= Results.count()) {
        emit bindsUpdated();
        return;
    }

    if (NewPosition < 0 || NewPosition >= Results.count()) {
        emit bindsUpdated();
        return;
    }

    LOG_INFO("Swap items %d %d", OldPosition, NewPosition);
    Results.swapItemsAt(OldPosition, NewPosition);
    NewPosition = -1;
    emit bindsUpdated();
}

QJsonObject CBModuleBindResultPool::GenerateConfig() const
{
    QJsonObject Config;
    QJsonArray ResultsJSON;
    for (const QSharedPointer<CBModuleBindResult>& Result : Results) {
        if (!Result.isNull()) {
            QJsonObject ResultConfig = Result->GenerateConfig();
            if (!ResultConfig.isEmpty()) {
                ResultsJSON.push_back(ResultConfig);
            }
        }
    }

    if (!ResultsJSON.isEmpty()) {
        Config.insert(POSTFIX, PoolPostfix);
        Config.insert(RESULTS, ResultsJSON);
    }

    return Config;
}

void CBModuleBindResultPool::ParseConfig(const QJsonObject& Config)
{
    if (Config.contains(POSTFIX) && Config.contains(RESULTS)) {
        if (Config[POSTFIX].toString() == PoolPostfix) {
            for (QJsonValueConstRef Value : Config[RESULTS].toArray()) {
                QString TargetAction = CBModuleBindResult::GetTargetActionNameFromConfig(Value.toObject());
                if (ActionsManager* Manager = CBAccessPoint::GetActionsManager()) {
                    if (CBModulesManager* ModulesManager = CBAccessPoint::GetModulesManager()) {
                        bindNewAction(Manager, ModulesManager, TargetAction);
                    }
                }
            }
        }
    }
}
