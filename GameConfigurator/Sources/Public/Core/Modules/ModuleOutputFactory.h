#pragma once

#include <QSharedPointer>
#include <QJsonObject>

class CBModuleOutput;

class CBModuleOutputFactory {
public:
    static QSharedPointer<CBModuleOutput> CreateOutput(const QJsonObject& OutputConfig);
};
