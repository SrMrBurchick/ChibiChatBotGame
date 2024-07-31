#include "Models/Twitch/ChannelPointsRewardsModel.h"
#include "System/Logger.h"
#include "Managers/TwitchManager.h"
#include "Core/Twitch/ChannelPointsReward.h"

#include <QQmlEngine>
#include <QTimer>

ChannelPointsRewardsModel::ChannelPointsRewardsModel(QObject* Parent)
    :BaseTwitchModel(Parent)
{
}

int ChannelPointsRewardsModel::rowCount(const QModelIndex& Parent) const
{
    Q_UNUSED(Parent);

    if (Manager)
    {
        return Manager->getChannelPointsRewardsCount();
    }

    return 0;
}

QVariant ChannelPointsRewardsModel::data(const QModelIndex& Index, int Role) const
{
    QVariant Data;

    if (Manager)
    {
        if (QSharedPointer<ChannelPointsReward> TargetReward = Manager->GetReward(Index.row())) {
            if (TargetReward.isNull()) {
                return Data;
            }

            switch (Role) {
                case eRewardsListRole::IDRole:
                    Data = QVariant::fromValue(TargetReward->RewardID);
                    break;
                default:
                    break;
            }
        }
    }

    return Data;
}

QHash<int, QByteArray> ChannelPointsRewardsModel::roleNames() const
{
    QHash<int, QByteArray> RoleNames;

    RoleNames[eRewardsListRole::IDRole] = "reward_id";

    return RoleNames;
}

void ChannelPointsRewardsModel::registerModel(const QString& ModuleName)
{
    QByteArray URI = ModuleName.toLocal8Bit();
    qmlRegisterType<ChannelPointsRewardsModel>(URI.data(), 1, 0, "ChannelPointsRewardsModel");
}

void ChannelPointsRewardsModel::updateData()
{
    if (Manager)
    {
        beginResetModel();
        endResetModel();
    }
}

void ChannelPointsRewardsModel::onChannelPointsRewardsUpdated()
{
    updateData();
}

void ChannelPointsRewardsModel::OnTargetSubscribed()
{
    BaseTwitchModel::OnTargetSubscribed();

    if (Manager) {
        QObject::connect(Manager, &TwitchManager::channelPointsRewardsUpdated, this, &ChannelPointsRewardsModel::onChannelPointsRewardsUpdated);
        if (Manager->getChannelPointsRewardsCount() > 0) {
            updateData();
        }
    }
}

void ChannelPointsRewardsModel::UnsubscribeFromTarget()
{
    if (Manager) {
        QObject::disconnect(Manager, &TwitchManager::channelPointsRewardsUpdated, this, &ChannelPointsRewardsModel::onChannelPointsRewardsUpdated);
    }

    BaseTwitchModel::UnsubscribeFromTarget();
}
