#pragma once

#include <QObject>
#include <QColor>

#include "Managers/TwitchNetworkInterface.h"

class ChannelPointsReward : public QObject, public ITwitchNetwork
{
    Q_OBJECT

    Q_PROPERTY(QString id MEMBER RewardID)
    Q_PROPERTY(QString title MEMBER Title NOTIFY titleUpdated)
    Q_PROPERTY(int cost MEMBER Cost NOTIFY costUpdated)
    Q_PROPERTY(bool enabled MEMBER bIsEnabled NOTIFY enabledUpdated)
    Q_PROPERTY(bool userInputRequired MEMBER bIsUserInputRequired NOTIFY userInputRequiredChanged)
    Q_PROPERTY(QString userInputPrompt MEMBER UserPrompt NOTIFY userPromptUpdated)

public:
    explicit ChannelPointsReward(QObject* Parent = nullptr);
    virtual ~ChannelPointsReward();

    QString RewardID;
    QString Title;
    QString UserPrompt;
    int Cost = 0;
    bool bIsEnabled = false;
    bool bIsUserInputRequired = false;

    virtual QJsonObject const GetNetworkData() const override;
    virtual void ParseReply(QNetworkReply* Reply) override;
    virtual void ParseJson(const QJsonValue& Value) override;

signals:
    void userInputRequiredChanged(bool isRequired);
    void titleUpdated(const QString& newTitle);
    void costUpdated(int newCost);
    void enabledUpdated(int newEnabled);
    void userPromptUpdated(const QString& newUserPrompt);
};
