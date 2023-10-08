#pragma once

#include <QAbstractListModel>
#include "Components/ActionComponent.h"

enum eActionsSequenceListRole {
    Column = Qt::UserRole,
    Row,
    Inverted
};

class AnimationSequenceModel : public QAbstractListModel
{
    Q_OBJECT

public:
    explicit AnimationSequenceModel(QObject* Parent = nullptr);
    virtual ~AnimationSequenceModel() = default;

    virtual int rowCount(const QModelIndex& Parent = QModelIndex()) const override;
    virtual QVariant data(const QModelIndex& Index, int Role = Qt::DisplayRole) const override;

    virtual QHash<int, QByteArray> roleNames() const override;

    static void registerModel(const QString& ModuleName);

    void updateData();

    Q_INVOKABLE void removeElement(int Index);
    Q_INVOKABLE void addNewAction(int Column, int Row);
    Q_INVOKABLE void toggleInverted(int Index);
    Q_INVOKABLE void placeItemAt(int SourceIndex, int TargetIndex);

protected:
    QList<ActionSequenceSprite> SpriteList;
};
