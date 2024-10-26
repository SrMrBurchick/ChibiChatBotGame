#include "Core/Modules/Binds/ModuleBindResultPool.h"

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
    }

    return nullptr;
}
