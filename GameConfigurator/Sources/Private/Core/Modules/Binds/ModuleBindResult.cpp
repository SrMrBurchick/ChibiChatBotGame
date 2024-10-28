#include "Core/Modules/Binds/ModuleBindResult.h"
#include "Core/Action.h"

#include <QQmlEngine>

CBModuleBindResult::CBModuleBindResult(QObject* Parent)
    : QObject(Parent)
{

}

CBModuleBindResult::~CBModuleBindResult()
{

}

QSharedPointer<CBModuleBindResult> CBModuleBindResult::CreateResult(const QSharedPointer<Action>& TargetAction)
{
    if (QSharedPointer<CBModuleBindResult> NewResult = QSharedPointer<CBModuleBindResult>::create()) {
        NewResult->TargetAction = TargetAction;
        return NewResult;
    }

    return nullptr;
}

Action* CBModuleBindResult::getTargetAction() const
{
    if (!TargetAction.isNull()) {
        QQmlEngine::setObjectOwnership(TargetAction.data(), QQmlEngine::CppOwnership);
        return TargetAction.data();
    }

    return nullptr;
}

QString CBModuleBindResult::getTargetActionName() const
{
    if (!TargetAction.isNull()) {
        return TargetAction->getName();
    }
    return "";
}


bool CBModuleBindResult::IsBindedTo(const QSharedPointer<Action> NewAction) const
{
    if (!TargetAction.isNull() && !NewAction.isNull()) {
        return TargetAction->getName() == NewAction->getName();
    }

    return false;
}
