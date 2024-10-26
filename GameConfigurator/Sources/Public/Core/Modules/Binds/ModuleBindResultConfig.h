#pragma once

#include <QObject>
#include <QSharedPointer>
#include <QVector>
#include "Core/Modules/Outputs/ModuleOutput.h"

class CBModuleBindResultPool;

class CBModuleBindResultConfig: public QObject {
    Q_OBJECT

    Q_PROPERTY(CBModuleOutput* targetOutput READ getTargetOutput)

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


private:
    QSharedPointer<CBModuleOutput> TargetOutput;
    QVector<QSharedPointer<CBModuleBindResultPool>> Pools;
};
