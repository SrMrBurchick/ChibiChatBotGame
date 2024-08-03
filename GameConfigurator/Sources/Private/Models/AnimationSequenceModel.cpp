#include "Models/AnimationSequenceModel.h"
#include "System/Logger.h"
#include "Managers/ActionsManager.h"
#include "Core/Action.h"

#include <QQmlEngine>


AnimationSequenceModel::AnimationSequenceModel(QObject* Parent)
    :BaseListModel(Parent)
{
}

int AnimationSequenceModel::rowCount(const QModelIndex& Parent) const
{
    Q_UNUSED(Parent)

    if (Manager != nullptr)
    {
        if (Action* SelectedAction = Manager->getSelectedAction())
        {
            return SelectedAction->getTotalSpritesCounts();
        }
    }

    return 0;
}

QVariant AnimationSequenceModel::data(const QModelIndex& Index, int Role) const
{
    QVariant Data;

    if (Manager != nullptr)
    {
        if (Action* SelectedAction = Manager->getSelectedAction())
        {
            ActionSequenceSprite Sprite = SelectedAction->GetSpriteById(Index.row());
            switch (Role) {
                case eActionsSequenceListRole::Column:
                    Data = QVariant::fromValue(Sprite.Column);
                    break;
                case eActionsSequenceListRole::Row:
                    Data = QVariant::fromValue(Sprite.Row);
                    break;
                case eActionsSequenceListRole::Inverted:
                    Data = QVariant::fromValue(Sprite.bInverted);
                    break;
                default:
                    break;
            }
        }
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

void AnimationSequenceModel::OnTargetSubscribed()
{
    if (Manager)
    {
        QObject::connect(Manager, &ActionsManager::actionSelected, this, &AnimationSequenceModel::actionSelected);
        if (Action* SelectedAction = Manager->getSelectedAction())
        {
            QObject::connect(SelectedAction, &Action::spriteSequenceUpdated, this, &AnimationSequenceModel::sequenceUpdated);
        }
    }

    BaseListModel::OnTargetSubscribed();
}

void AnimationSequenceModel::UnsubscribeFromTarget()
{
    if (Manager)
    {
        QObject::disconnect(Manager, &ActionsManager::actionSelected, this, &AnimationSequenceModel::actionSelected);
        if (Action* SelectedAction = Manager->getSelectedAction())
        {
            QObject::disconnect(SelectedAction, &Action::spriteSequenceUpdated, this, &AnimationSequenceModel::sequenceUpdated);
        }
    }

    BaseListModel::UnsubscribeFromTarget();
}

void AnimationSequenceModel::actionSelected(Action* selectedAction)
{
    if (Manager)
    {
        if (Action* SelectedAction = Manager->getSelectedAction())
        {
            beginRemoveRows(QModelIndex(), 0, SelectedAction->getTotalSpritesCounts());
            QObject::disconnect(SelectedAction, &Action::spriteSequenceUpdated, this, &AnimationSequenceModel::sequenceUpdated);
            endRemoveRows();
        }
    }

    if (selectedAction)
    {
        QObject::connect(selectedAction, &Action::spriteSequenceUpdated, this, &AnimationSequenceModel::sequenceUpdated);
    }
}

void AnimationSequenceModel::sequenceUpdated()
{
    beginResetModel();
    endResetModel();
}
