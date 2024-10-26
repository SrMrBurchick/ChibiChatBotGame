#include "Core/Modules/Outputs/ModuleOutput.h"
#include "Core/Modules/Binds/ModuleBindResultPool.h"

CBModuleOutput::CBModuleOutput(QObject* Parent)
    : QObject(Parent)
{

}

CBModuleOutput::~CBModuleOutput()
{

}
EOutputType CBModuleOutput::GetType() const
{
    return EOutputType::eUnknown;
}

int CBModuleOutput::GetPoolsCount() const
{
    return -1;
}

QSharedPointer<CBModuleBindResultPool> CBModuleOutput::CreatePool(int Index) const
{
    return nullptr;
}
