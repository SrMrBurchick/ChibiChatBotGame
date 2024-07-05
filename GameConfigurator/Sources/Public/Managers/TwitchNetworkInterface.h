#pragma once

#include <QUrl>
#include <QNetworkReply>

class ITwitchNetwork {
public:
    virtual ~ITwitchNetwork() = default;

    virtual const QUrl GetNetworkData() const = 0;
    virtual void ParseReply(QNetworkReply* Reply = nullptr) = 0;
    virtual void ParseJson(const QJsonValue& Value) = 0;
};
