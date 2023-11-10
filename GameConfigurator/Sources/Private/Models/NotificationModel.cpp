#include "Models/NotificationModel.h"

#include <QQmlEngine>
#include <iostream>

NotificationListModel::NotificationListModel(QObject* Parent)
    :QAbstractListModel(Parent)
{

}

int NotificationListModel::rowCount(const QModelIndex& Parent) const
{
    Q_UNUSED(Parent);

    return NotificationList.size();
}

QVariant NotificationListModel::data(const QModelIndex& Index, int Role) const
{
    QVariant Data;
    if (!Index.isValid() || Index.row() > rowCount(Index)) {
        return Data;
    }


    switch (Role) {
        case eNotificationListRole::Title:
            Data = QVariant::fromValue(NotificationList.at(Index.row()).Title);
            break;
        case eNotificationListRole::Body:
            Data = QVariant::fromValue(NotificationList.at(Index.row()).Body);
            break;
        default:
            break;
    }

    return Data;
}

QHash<int, QByteArray> NotificationListModel::roleNames() const
{
    QHash<int, QByteArray> RoleMaps;

    RoleMaps[eNotificationListRole::Title] = "title";
    RoleMaps[eNotificationListRole::Body] = "body";

    return RoleMaps;
}

void NotificationListModel::registerModel(const QString& ModuleName)
{
    QByteArray URI = ModuleName.toLocal8Bit();
    qmlRegisterType<NotificationListModel>(URI.data(), 1, 0, "NotificationListModel");
}

void NotificationListModel::updateData()
{
    beginResetModel();
    endResetModel();

    emit notificationsCountChanged(NotificationList.count());
}

void NotificationListModel::addNotification(const QString& Title, const QString& Body)
{
    if (Title.isEmpty() && Body.isEmpty()) {
        return;
    }

    Notification NewNotification {
        .Title = Title,
        .Body = Body
    };

    beginInsertRows(QModelIndex(), NotificationList.count(), NotificationList.count());
    NotificationList.push_back(NewNotification);
    endInsertRows();

    emit notificationsCountChanged(NotificationList.count());
}

void NotificationListModel::removeNotification(int Index)
{
    if (Index > NotificationList.size() || Index <= 0) {
        return;
    }

    beginRemoveRows(QModelIndex(), Index, Index);
    NotificationList.removeAt(Index);
    endRemoveRows();
    emit notificationsCountChanged(NotificationList.count());
}

int NotificationListModel::getNotificationsCount()
{
    return NotificationList.size();
}
