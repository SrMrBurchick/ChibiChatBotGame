#pragma once

#include "Core/Modules/Outputs/ModuleOutput.h"

class CBModuleBoolOutput: public CBModuleOutput {
    Q_OBJECT

public:
   // ================================ C++ ====================================
    // Modifiers
    explicit CBModuleBoolOutput(QObject* Parent = nullptr);
    virtual ~CBModuleBoolOutput();

    // Getters
    virtual EOutputType GetType() const override;
    virtual int GetPoolsCount() const override;
    virtual QSharedPointer<CBModuleBindResultPool> CreatePool(int Index) const override;
};
