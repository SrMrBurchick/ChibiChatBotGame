#pragma once

#include <QObject>
#include <QJsonObject>

class CBModuleInputParam: public QObject {
    Q_OBJECT

public:
    explicit CBModuleInputParam(QObject* Parent = nullptr);
    virtual ~CBModuleInputParam();
    bool ParseJson(const QJsonObject& Config);

private:
    QString Name;
    bool bIsRequired;
};
