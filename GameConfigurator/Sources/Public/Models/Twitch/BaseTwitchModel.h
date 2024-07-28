
#pragma once

#include <QAbstractListModel>

class TwitchManager;

class BaseTwitchModel : public QAbstractListModel
{
    Q_OBJECT

public:
    explicit BaseTwitchModel(QObject* Parent = nullptr);
    virtual ~BaseTwitchModel();

    Q_INVOKABLE void subscribeOnTarget(TwitchManager* Manager);

protected:
    virtual void OnTargetSubscribed();
    virtual void UnsubscribeFromTarget();

    TwitchManager* Manager = nullptr;
};
