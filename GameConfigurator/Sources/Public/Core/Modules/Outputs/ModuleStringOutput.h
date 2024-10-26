#pragma once

#include "Core/Modules/Outputs/ModuleOutput.h"

class CBModuleStringOutput: public CBModuleOutput {
    Q_OBJECT

public:
    // ================================ C++ ====================================
    // Modifiers
    explicit CBModuleStringOutput(QObject* Parent = nullptr);
    virtual ~CBModuleStringOutput();

    // Getters
    virtual EOutputType GetType() const override;
    virtual int GetPoolsCount() const override;
    virtual QSharedPointer<CBModuleBindResultPool> CreatePool(int Index) const override;
};
