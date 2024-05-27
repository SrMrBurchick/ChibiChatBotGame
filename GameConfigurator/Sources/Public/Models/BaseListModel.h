#pragma once

#include <QAbstractListModel>

class ActionsManager;

class BaseListModel : public QAbstractListModel
{
    Q_OBJECT

public:
    explicit BaseListModel(QObject* Parent = nullptr);
    virtual ~BaseListModel();

    Q_INVOKABLE void subscribeOnTarget(ActionsManager* Manager);

public slots:
    void onActionsUpdated();

protected:
    virtual void OnActionsUpdated();
    virtual void OnTargetSubscribed();

    virtual void UnsubscribeFromTarget();

    ActionsManager* Manager = nullptr;
};
