#pragma once

#include <QAbstractListModel>
#include <QObject>
#include <QVector>
#include "Components/ActionComponent.h"

enum eSpriteSheetRoles {
    SpriteColumn = Qt::UserRole,
    SpriteRow
};

class SpriteSheetModel : public QAbstractListModel
{
    Q_OBJECT

public:
    explicit SpriteSheetModel(QObject* Parent = nullptr);
    virtual ~SpriteSheetModel() = default;

    virtual int rowCount(const QModelIndex& Parent = QModelIndex()) const override;
    virtual QVariant data(const QModelIndex& Index, int Role = Qt::DisplayRole) const override;

    virtual QHash<int, QByteArray> roleNames() const override;

    static void registerModel(const QString& ModuleName);

    void updateData();

    Q_INVOKABLE void initModel(int Columns, int Rows);
    Q_INVOKABLE void clearModel();

protected:
    QVector<ActionSequenceSprite> SpriteSheet;
};
