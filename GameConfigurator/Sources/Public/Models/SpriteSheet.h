#pragma once

#include "Components/ActionComponent.h"
#include "Models/AnimationSequenceModel.h"

#include <QAbstractListModel>
#include <QObject>
#include <QVector>

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

    Q_INVOKABLE void initModel(int Columns, int Rows, AnimationSequenceModel* Model = nullptr);
    Q_INVOKABLE void clearModel();
    Q_INVOKABLE bool containsAction(const int Index, const QString& Action);
    Q_INVOKABLE void addAction(const int Index, const QString& Action);
    Q_INVOKABLE void removeAction(int Column, int Row, const QString& Action);
    Q_INVOKABLE void removeActionFromAll(const QString& Action);
    Q_INVOKABLE int getActionsCount(const int Index, const QString& Action) const;

protected:
    QVector<ActionGridSprite> SpriteSheet;
};
