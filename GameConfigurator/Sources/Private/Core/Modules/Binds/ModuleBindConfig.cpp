#include "Core/Modules/Binds/ModuleBindConfig.h"
#include "Core/Modules/Outputs/ModuleOutput.h"
#include "Core/Modules/Binds/ModuleBindResultConfig.h"

#include "Core/Action.h"


#include <QQmlEngine>

CBModuleBindConfig::CBModuleBindConfig(QObject* Parent)
    : QObject(Parent)
{

}

CBModuleBindConfig::~CBModuleBindConfig()
{

}

bool CBModuleBindConfig::IsBindedToAction(QSharedPointer<Action> NewAction) const
{
    if (!NewAction.isNull() && !TargetAction.isNull()) {
        return NewAction->getName() == TargetAction->getName();
    }

    return false;
}

QSharedPointer<CBModuleBindConfig> CBModuleBindConfig::CreateBindConfig(const QSharedPointer<Action>& NewAction, const QVector<QSharedPointer<CBModuleOutput>>& Outputs)
{
    if (QSharedPointer<CBModuleBindConfig> NewConfig = QSharedPointer<CBModuleBindConfig>::create()) {
        if (!NewAction.isNull()) {
            NewConfig->TargetAction = NewAction;
        }

        for (const QSharedPointer<CBModuleOutput>& Output : Outputs) {
            if (QSharedPointer<CBModuleBindResultConfig> Result = CBModuleBindResultConfig::CreateResultConfig(Output)) {
                NewConfig->Configs.push_back(Result);
            }
        }

        return NewConfig;
    }

    return nullptr;
}

Action* CBModuleBindConfig::getTargetAction() const
{
    if (!TargetAction.isNull()) {
        QQmlEngine::setObjectOwnership(TargetAction.data(), QQmlEngine::CppOwnership);
        return TargetAction.data();
    }

    return nullptr;
}

void CBModuleBindConfig::setSelected(bool bSelected)
{
    bIsSelected = bSelected;
    emit selectionChanged();
}

bool CBModuleBindConfig::IsSelected() const
{
    return bIsSelected;
}

CBModuleBindResultConfig* CBModuleBindConfig::getResultConfig(int Index) const
{
    if (Index < 0 && Index >= Configs.count()) {
        return nullptr;
    }

    if (QSharedPointer<CBModuleBindResultConfig> Config = Configs[Index]) {
        QQmlEngine::setObjectOwnership(Config.data(), QQmlEngine::CppOwnership);
        return Config.data();
    }

    return nullptr;
}

int CBModuleBindConfig::getResultsConfigCount() const
{
    return Configs.count();
}

