#include "Models/ActionsListModel.h"

#include <QQmlEngine>
#include <iostream>

ActionsListModel::ActionsListModel(QObject* Parent)
    :QAbstractListModel(Parent)
{
    // Standard actions
    ActionsList.push_back("Walk");
    ActionsList.push_back("Climb");
    ActionsList.push_back("StandBy");
    ActionsList.push_back("PickUp");
    ActionsList.push_back("Dance");
}

int ActionsListModel::rowCount(const QModelIndex& Parent) const
{
    Q_UNUSED(Parent);

    return ActionsList.size();
}

QVariant ActionsListModel::data(const QModelIndex& Index, int Role) const
{
    QVariant Data;
    if (!Index.isValid() || Index.row() > rowCount(Index)) {
        return Data;
    }


    switch (Role) {
        case eActionsListRole::NameRole:
            Data = QVariant::fromValue(ActionsList.at(Index.row()));
            break;
        default:
            break;
    }

    return Data;
}

QHash<int, QByteArray> ActionsListModel::roleNames() const
{
    QHash<int, QByteArray> RoleNames;

    RoleNames[eActionsListRole::NameRole] = "name";

    return RoleNames;
}

void ActionsListModel::registerModel(const QString& ModuleName)
{
    QByteArray URI = ModuleName.toLocal8Bit();
    qmlRegisterType<ActionsListModel>(URI.data(), 1, 0, "ActionsListModel");
}

void ActionsListModel::updateData()
{
    emit dataChanged(createIndex(0,0), createIndex(ActionsList.size(), 0));
}

void ActionsListModel::removeElement(int Index)
{
    if (Index > ActionsList.size() && Index <= 0) {
        return;
    }

    beginRemoveRows(QModelIndex(), Index, Index);
    ActionsList.removeAt(Index);
    endRemoveRows();
}

void ActionsListModel::changeElement(int Index, const QString& NewActionName)
{
    if (Index > ActionsList.size() && Index <= 0) {
        return;
    }

    std::cout << "Index = " << Index << " NewAction = " << NewActionName.toStdString() << std::endl;
    ActionsList[Index] = NewActionName;

    updateData();
}

void ActionsListModel::addNewAction(const QString& NewAction)
{
    beginInsertRows(QModelIndex(), ActionsList.size(), ActionsList.size());
    ActionsList.push_back(NewAction);
    endInsertRows();
}
