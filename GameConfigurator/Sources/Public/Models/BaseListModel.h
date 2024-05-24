#pragma once

#include <QAbstractListModel>

class ActionsManager;

class BaseListModel : public QAbstractListModel
{
    Q_OBJECT

public:
    explicit BaseListModel(QObject* Parent = nullptr);
    virtual ~BaseListModel() = default;

    Q_INVOKABLE void subscribeOnTarget(ActionsManager* Manager);

protected:
    virtual void OnActionsUpdated() = 0;
    virtual void OnTargetSubscribed() = 0;

    ActionsManager* Manager = nullptr;
};
