#include "Models/AnimationSequenceModel.h"

#include <QQmlEngine>
#include <iostream>

AnimationSequenceModel::AnimationSequenceModel(QObject* Parent)
    :QAbstractListModel(Parent)
{
    addNewAction(0, 0);
    addNewAction(0, 1);
    addNewAction(0, 2);
    addNewAction(0, 3);
    addNewAction(0, 4);
    addNewAction(1, 0);
    addNewAction(1, 1);
    addNewAction(1, 2);
    addNewAction(1, 3);
    addNewAction(1, 4);
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

    switch (Role) {
        case eActionsSequenceListRole::Column:
            Data = QVariant::fromValue(SpriteList.at(Index.row()).Column);
            break;
        case eActionsSequenceListRole::Row:
            Data = QVariant::fromValue(SpriteList.at(Index.row()).Row);
            break;
        case eActionsSequenceListRole::Inverted:
            Data = QVariant::fromValue(SpriteList.at(Index.row()).bInverted);
            break;
        default:
            break;
    }

    return Data;
}

QHash<int, QByteArray> AnimationSequenceModel::roleNames() const
{
    QHash<int, QByteArray> RoleNames;

    RoleNames[eActionsSequenceListRole::Column] = "column";
    RoleNames[eActionsSequenceListRole::Row] = "row";
    RoleNames[eActionsSequenceListRole::Inverted] = "inverted";

    return RoleNames;
}

void AnimationSequenceModel::registerModel(const QString& ModuleName)
{
    QByteArray URI = ModuleName.toLocal8Bit();
    qmlRegisterType<AnimationSequenceModel>(URI.data(), 1, 0, "AnimationSequenceModel");
}

void AnimationSequenceModel::updateData()
{
    emit dataChanged(createIndex(0,0), createIndex(SpriteList.size(), 0));
}

void AnimationSequenceModel::removeElement(int Index)
{
    if (Index > SpriteList.size() && Index <= 0) {
        return;
    }

    beginRemoveRows(QModelIndex(), Index, Index);
    SpriteList.removeAt(Index);
    endRemoveRows();
}

void AnimationSequenceModel::addNewAction(int Column, int Row)
{
    if (Column < 0 || Row < 0) {
        return;
    }

    SpriteList.push_back(ActionSequenceSprite(Column, Row));
}

void AnimationSequenceModel::toggleInverted(int Index)
{
    if (Index > SpriteList.size() && Index <= 0) {
        return;
    }

    ActionSequenceSprite* ActionSprite = &SpriteList[Index];
    if (nullptr != ActionSprite)
    {
        ActionSprite->toggleInverted();
    }
}

void AnimationSequenceModel::placeItemAt(int SourceIndex, int TargetIndex)
{
    if (SourceIndex == TargetIndex) {
        beginResetModel();
        endResetModel();

        return;
    }

    if (SourceIndex < 0 && SourceIndex > SpriteList.size()
        && TargetIndex < 0 && TargetIndex > SpriteList.size()) {
        return;
    }

    SpriteList.move(SourceIndex, TargetIndex);

    beginResetModel();
    endResetModel();
}
