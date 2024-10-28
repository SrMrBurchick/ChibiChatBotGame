#pragma once

#include <QObject>
#include <QSharedPointer>
#include <QVector>

class Action;

class CBModuleBindResult: public QObject {
    Q_OBJECT

    Q_PROPERTY(QString targetActionName READ getTargetActionName);

public:
    // ================================ C++ ====================================
    // Modifiers
    explicit CBModuleBindResult(QObject* Parent = nullptr);
    virtual ~CBModuleBindResult();

    static QSharedPointer<CBModuleBindResult> CreateResult(const QSharedPointer<Action>& TargetAction);

    // Getters
    virtual bool IsBindedTo(const QSharedPointer<Action> NewAction) const;

    // ================================ QML ====================================
    // Modifiers

    // Getters
    Q_INVOKABLE Action* getTargetAction() const;
    Q_INVOKABLE QString getTargetActionName() const;

private:
    QSharedPointer<Action> TargetAction;
};
