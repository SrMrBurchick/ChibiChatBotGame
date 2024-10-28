#include "Core/Modules/Outputs/ModuleBoolOutput.h"
#include "Core/Modules/Binds/ModuleBindResultPool.h"

#include "System/Logger.h"

constexpr bool ON_FALSE = 0;
constexpr bool ON_TRUE = 1;
constexpr char FALSE_POSTFIX[] = "OnFalse";
constexpr char TRUE_POSTFIX[] = "OnTrue";

CBModuleBoolOutput::CBModuleBoolOutput(QObject* Parent)
    : CBModuleOutput(Parent)
{

}

CBModuleBoolOutput::~CBModuleBoolOutput()
{

}

EOutputType CBModuleBoolOutput::GetType() const
{
    return EOutputType::eBool;
}

int CBModuleBoolOutput::GetPoolsCount() const
{
    return 2; // True / False pools
}

QSharedPointer<CBModuleBindResultPool> CBModuleBoolOutput::CreatePool(int Index) const
{
    LOG_INFO("Create bool pool");
    switch (Index) {
        case ON_FALSE:
            return CBModuleBindResultPool::CreatePool(FALSE_POSTFIX);
        case ON_TRUE:
            return CBModuleBindResultPool::CreatePool(TRUE_POSTFIX);
        default:
            break;
    }

    return nullptr;
}
