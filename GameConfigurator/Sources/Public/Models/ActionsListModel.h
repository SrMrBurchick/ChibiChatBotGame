#pragma once

#include <QAbstractListModel>
#include <QObject>
#include <QVector>

enum eActionsListRole {
    NameRole = Qt::UserRole
};

class ActionsListModel : public QAbstractListModel
{
    Q_OBJECT

public:
    explicit ActionsListModel(QObject* Parent = nullptr);
    virtual ~ActionsListModel() = default;

    virtual int rowCount(const QModelIndex& Parent = QModelIndex()) const override;
    virtual QVariant data(const QModelIndex& Index, int Role = Qt::DisplayRole) const override;

    virtual QHash<int, QByteArray> roleNames() const override;

    static void registerModel(const QString& ModuleName);

    void updateData();

    Q_INVOKABLE void removeElement(int Index);

protected:
    QVector<QString> ActionsList;
};