#include "Models/AnimationSequenceModel.h"

#include <QQmlEngine>
#include <iostream>

AnimationSequenceModel::AnimationSequenceModel(QObject* Parent)
    :QAbstractListModel(Parent)
{
}

int AnimationSequenceModel::rowCount(const QModelIndex& Parent) const
{
    Q_UNUSED(Parent)

    if (nullptr == SpriteList) {
        return 0;
    }

    return SpriteList->size();
}

QVariant AnimationSequenceModel::data(const QModelIndex& Index, int Role) const
{
    QVariant Data;
    if (!Index.isValid() || Index.row() > rowCount(Index)) {
        return Data;
    }

    if (nullptr == SpriteList) {
        return Data;
    }

    switch (Role) {
        case eActionsSequenceListRole::Column:
            Data = QVariant::fromValue(SpriteList->at(Index.row()).Column);
            break;
        case eActionsSequenceListRole::Row:
            Data = QVariant::fromValue(SpriteList->at(Index.row()).Row);
            break;
        case eActionsSequenceListRole::Inverted:
            Data = QVariant::fromValue(SpriteList->at(Index.row()).bInverted);
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
    if (nullptr != SpriteList) {
        emit dataChanged(createIndex(0,0), createIndex(SpriteList->size(), 0));
    }
}

void AnimationSequenceModel::removeElement(int Index)
{
    if (nullptr == SpriteList && Index > SpriteList->size() && Index <= 0) {
        return;
    }

    beginRemoveRows(QModelIndex(), Index, Index);
    SpriteList->removeAt(Index);
    endRemoveRows();
}

void AnimationSequenceModel::addNewAction(int Column, int Row)
{
    if (Column < 0 || Row < 0 || nullptr == SpriteList) {
        return;
    }

    beginResetModel();
    SpriteList->push_back(ActionSequenceSprite(Column, Row));
    endResetModel();
}

void AnimationSequenceModel::toggleInverted(int Index)
{
    if (nullptr == SpriteList && Index > SpriteList->size() && Index <= 0) {
        return;
    }

    ActionSequenceSprite* ActionSprite = &((*SpriteList)[Index]);
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

    if (nullptr == SpriteList && SourceIndex < 0 && SourceIndex > SpriteList->size()
        && TargetIndex < 0 && TargetIndex > SpriteList->size()) {
        return;
    }

    SpriteList->move(SourceIndex, TargetIndex);

    beginResetModel();
    endResetModel();
}

void AnimationSequenceModel::setActiveAction(const QString& Action)
{
    CurrentAction = Action;
    if (!ActionsMap.contains(CurrentAction)) {
        ActionsMap[Action] = QList<ActionSequenceSprite>();
    }

    SpriteList = &ActionsMap[CurrentAction];

    beginResetModel();
    endResetModel();
}

void AnimationSequenceModel::clearModel()
{
    SpriteList = nullptr;
    ActionsMap.clear();

    beginResetModel();
    endResetModel();
}
