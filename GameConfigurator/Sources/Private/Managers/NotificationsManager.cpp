#include "Managers/NotificationsManager.h"

QPointer<NotificationsManager> NotificationsManager::Manager = nullptr;

NotificationsManager::NotificationsManager(QObject* Parent)
    : QObject(Parent)
{

}

QPointer<NotificationsManager> NotificationsManager::GetManager()
{
    if (Manager.isNull()) {
        Manager = QPointer<NotificationsManager>(new NotificationsManager);
    }

    return Manager;
}

void NotificationsManager::notify(const QString& Title, const QString& Body)
{
    SendNotification(Title, Body);
}

void NotificationsManager::addModel(NotificationListModel* Model)
{
    if (Model) {
        NotificationModel = QPointer<NotificationListModel>(Model);
    }
}

void NotificationsManager::SendNotification(const QString& Title, const QString& Body)
{
    if (Manager.isNull()) {
        return;
    }

    if (Manager->NotificationModel.isNull()) {
        return;
    }

    Manager->NotificationModel->addNotification(Title, Body);
}

