#include "Models/ActionsListModel.h"

#include <QQmlEngine>
#include <QTimer>
#include <iostream>

ActionsListModel::ActionsListModel(QObject* Parent)
    :QAbstractListModel(Parent)
{
}

int ActionsListModel::rowCount(const QModelIndex& Parent) const
{
    Q_UNUSED(Parent);

    return ActionsList.size();
}

QVariant ActionsListModel::data(const QModelIndex& Index, int Role) const
{
    QVariant Data;
    if (!Index.isValid() || Index.row() > rowCount(Index) || ActionsList.isEmpty()) {
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
    if ((Index > ActionsList.size() && Index <= 0) || ActionsList.isEmpty()) {
        return;
    }

    beginRemoveRows(QModelIndex(), Index, Index);
    QString Action = ActionsList.takeAt(Index);
    endRemoveRows();

    emit actionRemoved(Action);
}

void ActionsListModel::changeElement(int Index, const QString& NewActionName)
{
    if ((Index > ActionsList.size() && Index <= 0) || ActionsList.isEmpty()) {
        return;
    }

    std::cout << "Index = " << Index << " NewAction = " << NewActionName.toStdString() << std::endl;
    ActionsList[Index] = NewActionName;

    updateData();
}

void ActionsListModel::addNewAction(const QString& NewAction)
{
    if (NewAction.isEmpty()) {
        return;
    }

    beginInsertRows(QModelIndex(), ActionsList.size(), ActionsList.size());
    ActionsList.push_back(NewAction);
    endInsertRows();

    if (ActionsList.size() == 1) {
        QTimer::singleShot(0, this, &ActionsListModel::setDefaultSelected);
    }

    emit actionAdded(NewAction);
}

int ActionsListModel::getSelectedActionIndex() const
{
    return SelectedActionIndex;
}

const QString ActionsListModel::getSelectedAction() const
{
    if (SelectedActionIndex >= ActionsList.size() || ActionsList.isEmpty()) {
        return QString();
    }

    return ActionsList.at(SelectedActionIndex);
}

void ActionsListModel::setSelectedActionIndex(int Index)
{
    if (Index >= ActionsList.size() || ActionsList.isEmpty() || Index < 0) {
        return;
    }

    SelectedActionIndex = Index;
    QString Action = ActionsList.at(SelectedActionIndex);

    emit actionSelected(Action);
}

void ActionsListModel::setDefaultSelected()
{
    if (ActionsList.isEmpty()) {
        return;
    }

    setSelectedActionIndex(0);
}

QVector<QString> ActionsListModel::getActions() const
{
    return ActionsList;
}
