#include "Core/Modules/Binds/ModuleBindResultConfig.h"
#include "Core/Modules/Outputs/ModuleOutput.h"
#include "Core/Modules/Binds/ModuleBindResultPool.h"

#include <QQmlEngine>

CBModuleBindResultConfig::CBModuleBindResultConfig(QObject* Parent)
    : QObject(Parent)
{

}

CBModuleBindResultConfig::~CBModuleBindResultConfig()
{

}

QSharedPointer<CBModuleBindResultConfig> CBModuleBindResultConfig::CreateResultConfig(const QSharedPointer<CBModuleOutput>& Output)
{
    if (QSharedPointer<CBModuleBindResultConfig> NewBind = QSharedPointer<CBModuleBindResultConfig>::create()) {
        if (!Output.isNull()) {
            NewBind->TargetOutput = Output;
            const int PoolsCount = Output->GetPoolsCount();
            if (PoolsCount > 0) {
                for (int Index = 0; Index < PoolsCount; ++Index) {
                    NewBind->Pools.push_back(Output->CreatePool(Index));
                }
            }

            return NewBind;
        }
    }

    return nullptr;
}

CBModuleOutput* CBModuleBindResultConfig::getTargetOutput() const
{
    if (!TargetOutput.isNull()) {
        QQmlEngine::setObjectOwnership(TargetOutput.data(), QQmlEngine::CppOwnership);
        return TargetOutput.data();
    }

    return nullptr;
}
