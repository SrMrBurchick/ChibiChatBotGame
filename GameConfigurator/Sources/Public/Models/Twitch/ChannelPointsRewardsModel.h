#pragma once

#include "Models/Twitch/BaseTwitchModel.h"

#include <QObject>

enum eRewardsListRole {
    IDRole = Qt::UserRole
};

class ChannelPointsRewardsModel : public BaseTwitchModel
{
    Q_OBJECT

public:
    explicit ChannelPointsRewardsModel(QObject* Parent = nullptr);
    virtual ~ChannelPointsRewardsModel() = default;

    virtual int rowCount(const QModelIndex& Parent = QModelIndex()) const override;
    virtual QVariant data(const QModelIndex& Index, int Role = Qt::DisplayRole) const override;

    virtual QHash<int, QByteArray> roleNames() const override;

    static void registerModel(const QString& ModuleName);

    void updateData();

protected slots:
    void onChannelPointsRewardsUpdated();

protected:
    virtual void OnTargetSubscribed() override;
    virtual void UnsubscribeFromTarget() override;
};
