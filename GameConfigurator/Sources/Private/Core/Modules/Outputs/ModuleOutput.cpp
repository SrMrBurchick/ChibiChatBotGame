#include "Core/Modules/Outputs/ModuleOutput.h"
#include "Core/Modules/Binds/ModuleBindResultPool.h"

#include "System/Logger.h"

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
    LOG_INFO("No Pools");
    return -1;
}

QSharedPointer<CBModuleBindResultPool> CBModuleOutput::CreatePool(int Index) const
{
    LOG_INFO("Cannot create pool");
    return nullptr;
}

const QString& CBModuleOutput::GetName() const
{
    return Name;
}

const QString CBModuleOutput::GetTypeString() const
{
    switch (GetType()) {
        case EOutputType::eBool:
            return "bool";
        case EOutputType::eNumber:
            return "number";
        case EOutputType::eString:
            return "string";
        default:
            break;
    }
    return "";
}
