#pragma once

#include <QObject>

class TwitchNetworkAccessManager;

class TwitchManager : public QObject
{
    Q_OBJECT

public:
    explicit TwitchManager(QObject* Parent = nullptr);
    virtual ~TwitchManager();

    // ================================ C++ ====================================
    // Modifiers
    void SetNetworkManager(TwitchNetworkAccessManager* Manager);


    // ================================ QML ====================================

private:
    TwitchNetworkAccessManager* NetworkManager;
};
