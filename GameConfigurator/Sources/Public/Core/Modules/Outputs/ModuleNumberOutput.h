#pragma once

#include "Core/Modules/Outputs/ModuleOutput.h"

class CBModuleNumberOutput: public CBModuleOutput {
    Q_OBJECT

public:
   // ================================ C++ ====================================
    // Modifiers
    explicit CBModuleNumberOutput(QObject* Parent = nullptr);
    virtual ~CBModuleNumberOutput();

    // Getters
    virtual EOutputType GetType() const override;
    virtual int GetPoolsCount() const override;
    virtual QSharedPointer<CBModuleBindResultPool> CreatePool(int Index) const override;
};
