#pragma once

#include <QObject>
#include <QColor>

#include "System/TwitchNetworkInterface.h"

class ChannelPointsReward : public QObject, public ITwitchNetwork
{
    Q_OBJECT

    Q_PROPERTY(QString id MEMBER m_ID)
    Q_PROPERTY(QString title MEMBER Title)
    Q_PROPERTY(int cost MEMBER Cost)
    Q_PROPERTY(bool enabled MEMBER bIsEnabled)
    Q_PROPERTY(bool userInputRequired MEMBER bIsUserInputRequired NOTIFY userInputRequiredChanged)
    Q_PROPERTY(QString userInputPrompt MEMBER UserPrompt)

public:
    explicit ChannelPointsReward(QObject* Parent = nullptr);
    virtual ~ChannelPointsReward() = default;

    QString m_ID = "";
    QString Title = "";
    QString UserPrompt = "";
    int Cost = 0;
    bool bIsEnabled = false;
    bool bIsUserInputRequired = false;

    virtual QUrl const GetNetworkData() const override;
    virtual void ParseReply(QNetworkReply* Reply) override;
    virtual void ParseJson(const QJsonValue& Value) override;

signals:
    void userInputRequiredChanged(bool isRequired);
};
