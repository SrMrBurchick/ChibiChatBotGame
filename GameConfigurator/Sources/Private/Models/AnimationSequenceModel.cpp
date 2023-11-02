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

    RoleNames[eActionsSequenceListRole::Column] = "sprite_column";
    RoleNames[eActionsSequenceListRole::Row] = "sprite_row";
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

    QMutexLocker MutexLocker(&SpriteListMutex);
    if (MutexLocker.isLocked()) {
        beginResetModel();
        SpriteList->push_back(ActionSequenceSprite(Column, Row));
        endResetModel();
    }
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

    QMutexLocker MutexLocker(&SpriteListMutex);
    if (MutexLocker.isLocked()) {
        SpriteList->move(SourceIndex, TargetIndex);
        currentSpriteIndex = 0;
    }

    beginResetModel();
    endResetModel();

}

void AnimationSequenceModel::setActiveAction(const QString& Action)
{
    CurrentAction = Action;
    if (!Map.contains(CurrentAction)) {
        Map[Action] = QList<ActionSequenceSprite>();
    }

    QMutexLocker MutexLocker(&SpriteListMutex);
    if (MutexLocker.isLocked()) {
        SpriteList = &Map[CurrentAction];
        currentSpriteIndex = 0;
    }

    beginResetModel();
    endResetModel();
}

void AnimationSequenceModel::clearModel()
{
    QMutexLocker MutexLocker(&SpriteListMutex);
    if (MutexLocker.isLocked()) {
        SpriteList = nullptr;
        currentSpriteIndex = 0;
        Map.clear();
    }

    beginResetModel();
    endResetModel();
}

QVariantMap AnimationSequenceModel::getNextSprite()
{
    QVariantMap NextSprite;
    QMutexLocker MutexLocker(&SpriteListMutex);

    if (SpriteList != nullptr && !SpriteList->isEmpty() && MutexLocker.isLocked()) {
        if (currentSpriteIndex >= SpriteList->length()) {
            currentSpriteIndex = 0;
        }

        NextSprite["sprite_column"] = SpriteList->at(currentSpriteIndex).Column;
        NextSprite["sprite_row"] = SpriteList->at(currentSpriteIndex).Row;
        NextSprite["isInverted"] = SpriteList->at(currentSpriteIndex).bInverted;
        ++currentSpriteIndex;
    }

    return NextSprite;
}

const ActionsMap& AnimationSequenceModel::getMap() const
{
    return Map;
}

void AnimationSequenceModel::initModel(const ActionsMap& InitMap)
{
    if (InitMap.isEmpty()) {
        return;
    }

    Map = InitMap;
    setActiveAction(Map.begin().key());
}

bool AnimationSequenceModel::isInverted(int Index) const
{
    if (SpriteList == nullptr) {
        return false;
    }

    if (SpriteList->length() <= Index) {
        return false;
    }

    return SpriteList->at(Index).bInverted;
}
