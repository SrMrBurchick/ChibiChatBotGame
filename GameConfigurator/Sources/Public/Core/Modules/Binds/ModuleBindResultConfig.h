#pragma once

#include <QObject>
#include <QSharedPointer>
#include <QVector>
#include "Core/Modules/Outputs/ModuleOutput.h"

class CBModuleBindResultPool;

class CBModuleBindResultConfig: public QObject {
    Q_OBJECT

    Q_PROPERTY(CBModuleOutput* targetOutput READ getTargetOutput)
    Q_PROPERTY(int poolsCount READ getPoolsCount)

public:
    // ================================ C++ ====================================
    // Modifiers
    explicit CBModuleBindResultConfig(QObject* Parent = nullptr);
    virtual ~CBModuleBindResultConfig();

    static QSharedPointer<CBModuleBindResultConfig> CreateResultConfig(const QSharedPointer<CBModuleOutput>& Output);

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
