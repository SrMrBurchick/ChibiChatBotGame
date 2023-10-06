#pragma once

#include <QJsonDocument>
#include <QString>

class ConfigLoader {
public:
    void LoadConfig(const QString& ConfigFile);
private:
    QString ConfigData;
    QJsonDocument ConfigObject;
};
