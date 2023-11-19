#include "Models/SpriteSheet.h"

#include <QQmlEngine>
#include <iostream>

SpriteSheetModel::SpriteSheetModel(QObject* Parent)
    :QAbstractListModel(Parent)
{
}

int SpriteSheetModel::rowCount(const QModelIndex& Parent) const
{
    Q_UNUSED(Parent)

    return SpriteSheet.size();
}

QVariant SpriteSheetModel::data(const QModelIndex& Index, int Role) const
{
    QVariant Data;
    if (!Index.isValid() || Index.row() > rowCount(Index)) {
        return Data;
    }

    switch (Role) {
        case eSpriteSheetRoles::SpriteColumn:
            Data = QVariant::fromValue(SpriteSheet.at(Index.row()).Column);
            break;
        case eSpriteSheetRoles::SpriteRow:
            Data = QVariant::fromValue(SpriteSheet.at(Index.row()).Row);
        default:
            break;
    }

    return Data;
}

QHash<int, QByteArray> SpriteSheetModel::roleNames() const
{
    QHash<int, QByteArray> RoleNames;

    RoleNames[eSpriteSheetRoles::SpriteColumn] = "sprite_column";
    RoleNames[eSpriteSheetRoles::SpriteRow] = "sprite_row";

    return RoleNames;
}

void SpriteSheetModel::registerModel(const QString& ModuleName)
{
    QByteArray URI = ModuleName.toLocal8Bit();
    qmlRegisterType<SpriteSheetModel>(URI.data(), 1, 0, "SpriteSheetModel");
}

void SpriteSheetModel::updateData()
{
    emit dataChanged(createIndex(0,0), createIndex(SpriteSheet.size(), 0));
}

void SpriteSheetModel::initModel(int Columns, int Rows, AnimationSequenceModel* Model)
{
    SpriteSheet.clear();

    for (int Row = 0; Row < Rows; Row++) {
        for (int Column = 0; Column < Columns; Column++) {
            ActionGridSprite GridSprite = ActionGridSprite(Column, Row);
            if (Model != nullptr) {
                Model->initSpriteActions(GridSprite);
            }
            SpriteSheet.push_back(GridSprite);
        }
    }

    beginResetModel();
    endResetModel();
}

void SpriteSheetModel::clearModel()
{
    SpriteSheet.clear();

    beginResetModel();
    endResetModel();
}

bool SpriteSheetModel::containsAction(const int Index, const QString& Action)
{
    if (Index >= SpriteSheet.size() || Index < 0) {
        return false;
    }

    return SpriteSheet.at(Index).Actions.contains(Action);
}

void SpriteSheetModel::addAction(const int Index, const QString& Action)
{
    if (Index >= SpriteSheet.size() || Index < 0) {
        return;
    }

    // Can be multiple copies of the sprite at the same action
    SpriteSheet[Index].Actions.push_back(Action);
}

void SpriteSheetModel::removeAction(int Column, int Row, const QString& Action)
{
    ActionGridSprite GridSprite(Column, Row);

    if (SpriteSheet.contains(GridSprite)) {
        int Index = SpriteSheet.indexOf(GridSprite);
        ActionGridSprite& Sprite = SpriteSheet[Index];

        if (Sprite.Actions.contains(Action)) {
            Sprite.Actions.removeAt(Sprite.Actions.indexOf(Action));
        }
    }
}

int SpriteSheetModel::getActionsCount(const int Index, const QString& Action) const
{
    if (Index >= SpriteSheet.size()) {
        return 0;
    }

    return SpriteSheet.at(Index).Actions.count(Action);
}

void SpriteSheetModel::removeActionFromAll(const QString& Action)
{
    if (SpriteSheet.isEmpty()) {
        return;
    }

    for (ActionGridSprite& Sprite: SpriteSheet) {
        if (Sprite.Actions.contains(Action)) {
            Sprite.Actions.removeIf([=](QString SpriteAction) {
                return Action == SpriteAction;
            });
        }
    }
}
