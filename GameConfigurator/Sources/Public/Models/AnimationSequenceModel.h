#pragma once

#include <QAbstractListModel>
#include <QMutex>

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

    void initModel(const ActionsMap& InitMap);

    Q_INVOKABLE void removeElement(int Index);
    Q_INVOKABLE void addNewAction(int Column, int Row);
    Q_INVOKABLE void toggleInverted(int Index);
    Q_INVOKABLE void placeItemAt(int SourceIndex, int TargetIndex);
    Q_INVOKABLE void setActiveAction(const QString& Action);
    Q_INVOKABLE void removeAction(const QString& Action);
    Q_INVOKABLE void clearModel();

    Q_INVOKABLE QVariantMap getNextSprite();
    Q_INVOKABLE const ActionsMap& getMap() const;
    Q_INVOKABLE bool isInverted(int Index) const;
    void initSpriteActions(ActionGridSprite& ActionSprite);

signals:
    void spriteRemoved(int Column, int Row);

protected:
    QList<ActionSequenceSprite>* SpriteList = nullptr;
    QString CurrentAction = "";
    int currentSpriteIndex = 0;
    ActionsMap Map;
    QMutex SpriteListMutex;
};
