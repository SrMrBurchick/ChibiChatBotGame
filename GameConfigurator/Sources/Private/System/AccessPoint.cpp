#include "System/AccessPoint.h"

#include "Managers/ActionsManager.h"
#include "Managers/TwitchManager.h"
#include "Managers/ModulesManger.h"
#include "System/HttpsServer.h"

#include <QQmlEngine>

QPointer<CBAccessPoint> CBAccessPoint::AccessPoint = nullptr;

CBAccessPoint::CBAccessPoint(QObject* Parent)
    : QObject(Parent)
{
    QmlEngine = nullptr;
}

QPointer<CBAccessPoint> CBAccessPoint::GetAccessPoint()
{
    if (AccessPoint.isNull()) {
        AccessPoint = QPointer<CBAccessPoint>(new CBAccessPoint);
    }

    return AccessPoint;
}

void CBAccessPoint::SetQMLEngine(QQmlEngine& Engine)
{
    QmlEngine = &Engine;
}

ActionsManager* CBAccessPoint::GetActionsManager()
{
    if (QPointer<CBAccessPoint> AccessPoint = GetAccessPoint()) {
        if (QQmlEngine* Engine = AccessPoint->QmlEngine) {
            return Engine->singletonInstance<ActionsManager*>("Managers", "ActionsManager");
        }
    }

    return nullptr;
}

TwitchManager* CBAccessPoint::GetTwitchManager()
{
    if (QPointer<CBAccessPoint> AccessPoint = GetAccessPoint()) {
        if (QQmlEngine* Engine = AccessPoint->QmlEngine) {
            return Engine->singletonInstance<TwitchManager*>("Managers", "TwitchManager");
        }
    }

    return nullptr;
}

CBModulesManager* CBAccessPoint::GetModulesManager()
{
    if (QPointer<CBAccessPoint> AccessPoint = GetAccessPoint()) {
        if (QQmlEngine* Engine = AccessPoint->QmlEngine) {
            return Engine->singletonInstance<CBModulesManager*>("Managers", "ModulesManager");
        }
    }

    return nullptr;
}

CBHttpsServer* CBAccessPoint::GetHttpsServer()
{
    if (QPointer<CBAccessPoint> AccessPoint = GetAccessPoint()) {
        if (QQmlEngine* Engine = AccessPoint->QmlEngine) {
            return Engine->singletonInstance<CBHttpsServer*>("Managers", "HttpsServer");
        }
    }

    return nullptr;
}

