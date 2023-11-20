#include "Models/PredefinedActionsModel.h"
#include "System/Logger.h"

#include <QQmlEngine>

PredefinedActionsListModel::PredefinedActionsListModel(QObject* Parent)
    :QAbstractListModel(Parent)
{

}

int PredefinedActionsListModel::rowCount(const QModelIndex& Parent) const
{
    Q_UNUSED(Parent);
    return PredefinedActionsList.count();
}

QVariant PredefinedActionsListModel::data(const QModelIndex& Index, int Role) const
{
    QVariant Data;

    if (Index.row() >= PredefinedActionsList.count()) {
        return Data;
    }

    switch (Role) {
        case ePredefinedActionsListRole::ActionNameRole:
            Data = QVariant::fromValue(PredefinedActionsList.at(Index.row()).ActionName);
            break;
        case ePredefinedActionsListRole::ChanceRole:
            Data = QVariant::fromValue(PredefinedActionsList.at(Index.row()).Chance);
            break;
    }

    return Data;
}

QHash<int, QByteArray> PredefinedActionsListModel::roleNames() const
{
    QHash<int, QByteArray> RoleNames;

    RoleNames[ePredefinedActionsListRole::ActionNameRole] = "name";
    RoleNames[ePredefinedActionsListRole::ChanceRole] = "chance";

    return RoleNames;
}

void PredefinedActionsListModel::registerModel(const QString& ModelName)
{
    QByteArray URI = ModelName.toLocal8Bit();
    qmlRegisterType<PredefinedActionsListModel>(URI.data(), 1, 0, "PredefinedActionsModel");
}

void PredefinedActionsListModel::removeElement(const int Index)
{
    if (Index > PredefinedActionsList.count() && Index <= 0) {
        return;
    }

    beginRemoveRows(QModelIndex(), Index, Index);
    PredefinedAction Action = PredefinedActionsList.takeAt(Index);
    endRemoveRows();

    emit actionRemoved(Action.ActionName);
}

void PredefinedActionsListModel::addNewAction(const QString& NewAction, const int Chance)
{
    if (NewAction.isEmpty()) {
        return;
    }

    LOG_INFO("Add new predefined action: %s, with chance %d", NewAction.toStdString().c_str(), Chance);

    beginInsertRows(QModelIndex(), PredefinedActionsList.count(), PredefinedActionsList.count());
    PredefinedActionsList.push_back(
        PredefinedAction {
            .ActionName = NewAction,
            .Chance = Chance
        }
    );
    endInsertRows();

    emit actionAdded(NewAction);
}

const QVector<PredefinedAction>& PredefinedActionsListModel::getList() const
{
    return PredefinedActionsList;
}

void PredefinedActionsListModel::initModel(QVector<PredefinedAction>& ActionsList)
{
    if (!PredefinedActionsList.isEmpty()) {
        PredefinedActionsList.clear();
    }

    PredefinedActionsList = ActionsList;
}

QVector<QString> PredefinedActionsListModel::getActions() const
{
    QVector<QString> Actions;

    for (const PredefinedAction& Action : PredefinedActionsList) {
        Actions.push_back(Action.ActionName);
    }

    return Actions;
}

void PredefinedActionsListModel::updateChance(const int Index, const int Chance)
{
    if (Index < 0 || PredefinedActionsList.isEmpty() || PredefinedActionsList.count() < Index) {
        return;
    }

    PredefinedActionsList[Index].Chance = Chance;
}
