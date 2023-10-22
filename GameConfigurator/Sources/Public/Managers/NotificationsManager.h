#pragma once
#include <QObject>
#include <QPointer>

#include "Models/NotificationModel.h"

class NotificationsManager: public QObject {
    Q_OBJECT

public:
    explicit NotificationsManager(QObject* Parent = nullptr);
    virtual ~NotificationsManager() = default;

    Q_INVOKABLE void notify(const QString& Title, const QString& Body);
    Q_INVOKABLE void addModel(NotificationListModel* Model);

    static void SendNotification(const QString& Title, const QString& Body);
    static QPointer<NotificationsManager> GetManager();

private:
    QPointer<NotificationListModel> NotificationModel;
    static QPointer<NotificationsManager> Manager;
};
