#include "Models/ActionsListModel.h"
#include "System/Logger.h"
#include "Managers/ActionsManager.h"
#include "Core/Action.h"

#include <QQmlEngine>
#include <QTimer>

ActionsListModel::ActionsListModel(QObject* Parent)
    :BaseListModel(Parent)
{
}

int ActionsListModel::rowCount(const QModelIndex& Parent) const
{
    Q_UNUSED(Parent);

    if (Manager)
    {
        return Manager->getActionsCount();
    }

    return -1;
}

QVariant ActionsListModel::data(const QModelIndex& Index, int Role) const
{
    QVariant Data;

    if (Manager)
    {
        if (QSharedPointer<Action> TargetAction = Manager->GetActionById(Index.row()))
        {
            if (TargetAction.isNull())
            {
                return Data;
            }

            switch (Role) {
                case eActionsListRole::NameRole:
                    Data = QVariant::fromValue(TargetAction->getName());
                    break;
                default:
                    break;
            }
        }
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
    if (Manager)
    {
        emit dataChanged(createIndex(0,0), createIndex(Manager->getActionsCount(), 0));
    }
}
