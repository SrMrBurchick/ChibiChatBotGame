#include "Models/AnimationSequenceModel.h"

#include <QQmlEngine>

AnimationSequenceModel::AnimationSequenceModel(QObject* Parent)
    :QAbstractListModel(Parent)
{

}

int AnimationSequenceModel::rowCount(const QModelIndex& Parent) const
{
    Q_UNUSED(Parent)

    return SpriteList.size();
}

QVariant AnimationSequenceModel::data(const QModelIndex& Index, int Role) const
{
    QVariant Data;
    if (!Index.isValid() || Index.row() > rowCount(Index)) {
        return Data;
    }

    // switch (Role) {
    //     case eActionsListRole::NameRole:
    //         Data = QVariant::fromValue(ActionsList.at(Index.row()));
    //         break;
    //     default:
    //         break;
    // }

    return Data;

}

QHash<int, QByteArray> AnimationSequenceModel::roleNames() const
{
    QHash<int, QByteArray> RoleNames;

    // RoleNames[eActionsListRole::NameRole] = "name";

    return RoleNames;

}

void AnimationSequenceModel::registerModel(const QString& ModuleName)
{
    QByteArray URI = ModuleName.toLocal8Bit();
    // qmlRegisterType<ActionsListModel>(URI.data(), 1, 0, "ActionsListModel");
}

void AnimationSequenceModel::updateData()
{
    // emit dataChanged(createIndex(0,0), createIndex(ActionsList.size(), 0));
}

void AnimationSequenceModel::removeElement(int Index)
{
    // if (Index > ActionsList.size() && Index <= 0) {
    //     return;
    // }
    //
    // beginRemoveRows(QModelIndex(), Index, Index);
    // ActionsList.removeAt(Index);
    // endRemoveRows();
}

void AnimationSequenceModel::addNewAction(int Column, int Row)
{
    if (Column < 0 || Row < 0) {
        return;
    }
    //
    // beginInsertRows(QModelIndex(), ActionsList.size(), ActionsList.size());
    // ActionsList.push_back(NewAction);
    // endInsertRows();

}

void AnimationSequenceModel::toggleInverted(int Index)
{
    if (Index > SpriteList.size() && Index <= 0) {
        return;
    }

    ActionSequenceSprite* ActionSprite = &SpriteList[Index];
    if (nullptr != ActionSprite)
    {
        ActionSprite->bInverted = !ActionSprite->bInverted;
    }
}
