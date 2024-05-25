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

    // ============================== QML ======================================
    // Modifiers
    Q_INVOKABLE void addNewAction(const QString& ActionName);
    Q_INVOKABLE void removeActionById(int Index);
    Q_INVOKABLE void changeActionName(int Index, const QString& NewName);

    // Getters
    Q_INVOKABLE int getActionsCount() const;
    Q_INVOKABLE Action* getActionByName(const QString& ActionName) const;
    Q_INVOKABLE Action* getActionById(int Index) const;

    Q_INVOKABLE bool isDefaultAction(Action* ActionToCheck) const;
    Q_INVOKABLE bool isDefaultAction(int Index) const;

    Q_INVOKABLE bool isGameDefaultAction(Action* ActionToCheck) const;
    Q_INVOKABLE bool isTwitchDefaultAction(Action* ActionToCheck) const;

    Q_INVOKABLE const QVector<QString>& getGameDefaultActions() const;
    Q_INVOKABLE const QVector<QString>& getTwitchDefaultAction() const;
    Q_INVOKABLE const QVector<QString> getPossibleActionsToAdd() const;

    Q_INVOKABLE Action* getSelectedAction() const;

    // Events
    Q_INVOKABLE void markSelectedAction(int Index);

    // ============================== C++ ======================================
    // Modifiers
    void removeAction(QSharedPointer<Action> ActionToRemove);

    // Getters
    QSharedPointer<Action> GetActionByName(const QString& ActionName) const;
    QSharedPointer<Action> GetActionById(int Index) const;
    const QVector<QSharedPointer<Action>>& getActions() const;

signals:
    void actionSelected(Action* selectedAction);
    void actionsUpdated();

private:
    QVector<QSharedPointer<Action>> Actions;
    QSharedPointer<Action> SelectedAction;

    // Default actions
    QVector<QString> GameDefaultActions = {"walk", "fall", "climb", "standby"};
    QVector<QString> TwitchDefaultActions = {
        "subscription", "subscriptionsGift", "raid", "ban", "follow", "moderatorAdded", "moderatorRemoved"
    };
};
