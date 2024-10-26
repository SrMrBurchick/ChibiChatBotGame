#pragma once

#include <QObject>
#include <QSharedPointer>

class Action;

class CBModuleBindResult: public QObject {
    Q_OBJECT

public:
    // ================================ C++ ====================================
    // Modifiers
    explicit CBModuleBindResult(QObject* Parent = nullptr);
    virtual ~CBModuleBindResult();

private:

    QSharedPointer<Action> BindAction;
};
