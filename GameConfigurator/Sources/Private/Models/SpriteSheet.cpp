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

void SpriteSheetModel::initModel(int Columns, int Rows)
{
    SpriteSheet.clear();

    for (int Row = 0; Row < Rows; Row++) {
        for (int Column = 0; Column < Columns; Column++) {
            SpriteSheet.push_back(ActionGridSprite(Column, Row));
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
