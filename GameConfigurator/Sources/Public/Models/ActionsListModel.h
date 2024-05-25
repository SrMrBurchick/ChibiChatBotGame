#pragma once

#include "Models/BaseListModel.h"

#include <QObject>
#include <QVector>


enum eActionsListRole {
    NameRole = Qt::UserRole
};

class ActionsListModel : public BaseListModel
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
    Q_INVOKABLE void changeElement(int Index, const QString& NewActionName);
    Q_INVOKABLE void addNewAction(const QString& NewAction);
    Q_INVOKABLE int getSelectedActionIndex() const;
    Q_INVOKABLE const QString getSelectedAction() const;
    Q_INVOKABLE void setSelectedActionIndex(int Index);
    Q_INVOKABLE void setDefaultSelected();
    Q_INVOKABLE QVector<QString> getActions() const;

signals:
    void actionSelected(QString Action);
    void actionRemoved(QString Action);
    void actionAdded(QString Action);

protected:
    virtual void OnActionsUpdated() override;
    virtual void OnTargetSubscribed() override;

protected:
    QVector<QString> ActionsList;
    int SelectedActionIndex = -1;
};
