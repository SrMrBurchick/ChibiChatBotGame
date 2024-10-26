#include "Core/Modules/Outputs/ModuleStringOutput.h"
#include "Core/Modules/Binds/ModuleBindResultPool.h"

constexpr char STRING_POSTFIX[] = "String";

CBModuleStringOutput::CBModuleStringOutput(QObject* Parent)
    : CBModuleOutput(Parent)
{

}

CBModuleStringOutput::~CBModuleStringOutput()
{

}

EOutputType CBModuleStringOutput::GetType() const
{
    return EOutputType::eString;
}

int CBModuleStringOutput::GetPoolsCount() const
{
    return 1;
}

QSharedPointer<CBModuleBindResultPool> CBModuleStringOutput::CreatePool(int Index) const
{
    return CBModuleBindResultPool::CreatePool(STRING_POSTFIX);
}
