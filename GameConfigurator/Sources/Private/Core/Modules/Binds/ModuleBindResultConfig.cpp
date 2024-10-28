#include "Core/Modules/Binds/ModuleBindResultConfig.h"
#include "Core/Modules/Outputs/ModuleOutput.h"
#include "Core/Modules/Binds/ModuleBindResultPool.h"

#include "System/Logger.h"

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
            for (int Index = 0; Index < Output->GetPoolsCount(); ++Index) {
                if (QSharedPointer<CBModuleBindResultPool> NewPool = Output->CreatePool(Index)) {
                    NewBind->Pools.push_back(NewPool);
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

int CBModuleBindResultConfig::getPoolsCount() const
{
    return Pools.count();
}

CBModuleBindResultPool* CBModuleBindResultConfig::getPool(int Index) const
{
    LOG_INFO("Try to get pool by index %d, PoolsCount = %d", Index, Pools.count());
    if (Index < 0 || Index >= Pools.count()) {
        return nullptr;
    }

    if (QSharedPointer<CBModuleBindResultPool> Pool = Pools[Index]) {
        LOG_INFO("Select pool = %s", Pool->getPoolPostfix().toStdString().c_str());
        QQmlEngine::setObjectOwnership(Pool.data(), QQmlEngine::CppOwnership);
        return Pool.data();
    }

    return nullptr;
}
