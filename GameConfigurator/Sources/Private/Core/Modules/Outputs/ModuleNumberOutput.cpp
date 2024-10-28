#include "Core/Modules/Outputs/ModuleNumberOutput.h"
#include "Core/Modules/Binds/ModuleBindResultPool.h"
#include "System/Logger.h"

constexpr char NUMBER_POSTFIX [] = "Number";

CBModuleNumberOutput::CBModuleNumberOutput(QObject* Parent)
    : CBModuleOutput(Parent)
{

}

CBModuleNumberOutput::~CBModuleNumberOutput()
{

}

EOutputType CBModuleNumberOutput::GetType() const
{
    return EOutputType::eNumber;
}

int CBModuleNumberOutput::GetPoolsCount() const
{
    return 1;
}

QSharedPointer<CBModuleBindResultPool> CBModuleNumberOutput::CreatePool(int Index) const
{
    LOG_INFO("Create number pool");
    return CBModuleBindResultPool::CreatePool(NUMBER_POSTFIX);
}
