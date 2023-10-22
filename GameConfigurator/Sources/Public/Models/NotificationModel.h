#pragma once

#include <QAbstractListModel>
#include <QObject>
#include <QVector>

#include "Components/NotificationComponent.h"

enum eNotificationListRole {
    Title = Qt::UserRole,
    Body
};

class NotificationListModel : public QAbstractListModel
{
    Q_OBJECT

public:
    explicit NotificationListModel(QObject* Parent = nullptr);
    virtual ~NotificationListModel() = default;

    virtual int rowCount(const QModelIndex& Parent = QModelIndex()) const override;
    virtual QVariant data(const QModelIndex& Index, int Role = Qt::DisplayRole) const override;

    virtual QHash<int, QByteArray> roleNames() const override;

    static void registerModel(const QString& ModuleName);

    void updateData();

    Q_INVOKABLE void addNotification(const QString& Title, const QString& Body);
    Q_INVOKABLE void removeNotification(int Index);

protected:
    QVector<Notification> NotificationList;
};
