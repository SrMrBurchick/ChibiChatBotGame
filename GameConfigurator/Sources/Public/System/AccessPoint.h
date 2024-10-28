#pragma once
#include <QObject>
#include <QPointer>


class ActionsManager;
class TwitchManager;
class CBModulesManager;
class QQmlEngine;

class CBAccessPoint: public QObject {
    Q_OBJECT

public:
    explicit CBAccessPoint(QObject* Parent = nullptr);
    virtual ~CBAccessPoint() = default;

    void SetQMLEngine(QQmlEngine& Engine);

    static QPointer<CBAccessPoint> GetAccessPoint();

    static ActionsManager* GetActionsManager();
    static TwitchManager* GetTwitchManager();
    static CBModulesManager* GetModulesManager();

protected:

    static QPointer<CBAccessPoint> AccessPoint;
    QQmlEngine* QmlEngine;
};
