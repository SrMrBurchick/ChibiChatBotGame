#include "Models/ActionsListModel.h"
#include "System/Logger.h"

#include <QQmlEngine>
#include <QTimer>
#include <functional>

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
    if ((Index > ActionsList.size() && Index < 0) || ActionsList.isEmpty()) {
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

    LOG_INFO("Change name to: %s, at: %d", NewActionName.toStdString().c_str(), Index);

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

    LOG_INFO("New action was added: %s", NewAction.toStdString().c_str());

    emit actionAdded(NewAction);
}

int ActionsListModel::getSelectedActionIndex() const
{
    return SelectedActionIndex;
}

const QString ActionsListModel::getSelectedAction() const
{
    if (SelectedActionIndex < 0) {
        LOG_CRITICAL("Was so close to sig 11");
        return QString();
    }

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

    if (Index == SelectedActionIndex) {
        return;
    }

    SelectedActionIndex = Index;
    QString Action = ActionsList.at(SelectedActionIndex);

    LOG_INFO("Select action: %s", Action.toStdString().c_str());

    emit actionSelected(Action);
}

void ActionsListModel::setDefaultSelected()
{
    if (ActionsList.isEmpty()) {
        return;
    }

     if (ActionsList.size() == 1) {
         QTimer::singleShot(1, this, std::bind(&ActionsListModel::setSelectedActionIndex, this, 0));
     }
}

QVector<QString> ActionsListModel::getActions() const
{
    return ActionsList;
}
