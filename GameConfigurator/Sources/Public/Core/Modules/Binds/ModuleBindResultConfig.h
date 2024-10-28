#pragma once

#include <QObject>
#include <QSharedPointer>
#include <QVector>
#include "Core/Modules/Outputs/ModuleOutput.h"

class CBModuleBindResultPool;
class QJsonObject;

class CBModuleBindResultConfig: public QObject {
    Q_OBJECT

    Q_PROPERTY(CBModuleOutput* targetOutput READ getTargetOutput)
    Q_PROPERTY(int poolsCount READ getPoolsCount)

public:
    // ================================ C++ ====================================
    // Modifiers
    explicit CBModuleBindResultConfig(QObject* Parent = nullptr);
    virtual ~CBModuleBindResultConfig();

    QJsonObject GenerateConfig() const;
    void ParseConfig(const QJsonObject& Config);
    static QSharedPointer<CBModuleBindResultConfig> CreateResultConfig(const QSharedPointer<CBModuleOutput>& Output);

    // Getters
    QSharedPointer<CBModuleBindResultPool> GetPoolByPostfix(const QString& Postfix);
    bool HasTargetOutputFromConfig(const QJsonObject& Config);

    // ================================ QML ====================================
    // Modifiers

    // Getters
    Q_INVOKABLE CBModuleOutput* getTargetOutput() const;
    Q_INVOKABLE int getPoolsCount() const;
    Q_INVOKABLE CBModuleBindResultPool* getPool(int Index) const;

private:
    QSharedPointer<CBModuleOutput> TargetOutput;
    QVector<QSharedPointer<CBModuleBindResultPool>> Pools;
};
