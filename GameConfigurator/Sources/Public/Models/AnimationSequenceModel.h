#pragma once

#include "Models/BaseListModel.h"

class Action;

enum eActionsSequenceListRole {
    Column = Qt::UserRole,
    Row,
    Inverted
};

class AnimationSequenceModel : public BaseListModel
{
    Q_OBJECT

public:
    explicit AnimationSequenceModel(QObject* Parent = nullptr);
    virtual ~AnimationSequenceModel() = default;

    virtual int rowCount(const QModelIndex& Parent = QModelIndex()) const override;
    virtual QVariant data(const QModelIndex& Index, int Role = Qt::DisplayRole) const override;

    virtual QHash<int, QByteArray> roleNames() const override;

    static void registerModel(const QString& ModuleName);

protected:
    virtual void OnTargetSubscribed() override;
    virtual void UnsubscribeFromTarget() override;

public slots:
    void actionSelected(Action* selectedAction);
    void sequenceUpdated();
};
