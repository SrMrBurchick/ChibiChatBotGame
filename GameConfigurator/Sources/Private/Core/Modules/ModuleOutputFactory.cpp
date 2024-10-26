#include "Core/Modules/ModuleOutputFactory.h"
#include "Core/Modules/Outputs/ModuleOutput.h"
#include "Core/Modules/Outputs/ModuleBoolOutput.h"
#include "Core/Modules/Outputs/ModuleNumberOutput.h"
#include "Core/Modules/Outputs/ModuleStringOutput.h"

constexpr char NAME[] = "name";
constexpr char TYPE[] = "type";
constexpr char TYPE_BOOL[] = "bool";
constexpr char TYPE_NUMBER[] = "number";
constexpr char TYPE_STRING[] = "string";

QSharedPointer<CBModuleOutput> CBModuleOutputFactory::CreateOutput(const QJsonObject& OutputConfig)
{
    QSharedPointer<CBModuleOutput> Output;
    if (OutputConfig.contains(TYPE))
    {
        QString OutputType = OutputConfig.value(TYPE).toString().toLower();
        if (OutputType == TYPE_BOOL) {
            Output = QSharedPointer<CBModuleBoolOutput>::create();
        } else if (OutputType == TYPE_NUMBER) {
            Output = QSharedPointer<CBModuleNumberOutput>::create();
        } else if (OutputType == TYPE_STRING) {
            Output = QSharedPointer<CBModuleStringOutput>::create();
        }
    }

    if (!Output.isNull()) {
        if (OutputConfig.contains(NAME)) {
            Output->Name = OutputConfig.value(NAME).toString();
        }
    }

    return Output;
}
