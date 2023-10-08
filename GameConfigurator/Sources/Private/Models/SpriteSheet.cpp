#include "Models/SpriteSheet.h"

#include <QQmlEngine>

SpriteSheetModel::SpriteSheetModel(QObject* Parent)
    :QAbstractListModel(Parent)
{
}

int SpriteSheetModel::rowCount(const QModelIndex& Parent) const
{
    Q_UNUSED(Parent)

    return 0;
}

QVariant SpriteSheetModel::data(const QModelIndex& Index, int Role) const
{
    QVariant Data;
    if (!Index.isValid() || Index.row() > rowCount(Index)) {
        return Data;
    }

    return Data;
}

QHash<int, QByteArray> SpriteSheetModel::roleNames() const
{
    QHash<int, QByteArray> RoleNames;

    return RoleNames;
}

void SpriteSheetModel::registerModel(const QString& ModuleName)
{
    QByteArray URI = ModuleName.toLocal8Bit();
    qmlRegisterType<SpriteSheetModel>(URI.data(), 1, 0, "SpriteSheetModel");
}

void SpriteSheetModel::updateData()
{
}
