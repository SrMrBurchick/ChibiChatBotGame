#pragma once

#include <QObject>
#include <QSharedPointer>

class Action;

class ActionsManager : public QObject
{
    Q_OBJECT

public:
    explicit ActionsManager(QObject* Parent = nullptr);
    virtual ~ActionsManager() = default;

    Q_INVOKABLE void addNewAction(const QString& ActionName);
    Q_INVOKABLE void removeAction(QSharedPointer<Action> ActionToRemove);
    Q_INVOKABLE void removeActionById(int Index);

    Q_INVOKABLE QSharedPointer<Action> getActionByName(const QString& ActionName) const;
    Q_INVOKABLE QSharedPointer<Action> getActionById(int Index) const;

    Q_INVOKABLE const QVector<QSharedPointer<Action>>& getActions() const;
    Q_INVOKABLE int getActionsCount() const;

signals:
    void actionSelected(QSharedPointer<Action>);
    void actionsUpdated();

private:
    QVector<QSharedPointer<Action>> Actions;
};
