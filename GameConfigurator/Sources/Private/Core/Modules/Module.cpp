#include "Core/Modules/Module.h"
#include "Core/Modules/Outputs/ModuleOutput.h"
#include "Core/Modules/ModuleOutputFactory.h"
#include "Core/Modules/Binds/ModuleBindConfig.h"
#include "Core/Action.h"
#include "Managers/ActionsManager.h"

#include "System/Logger.h"

#include <QJsonArray>
#include <QJsonObject>
#include <QJsonValue>
#include <QQmlEngine>

constexpr char NAME[] = "name";
constexpr char VERSION[] = "version";
constexpr char PATH[] = "path";
constexpr char INPUT_PARAMS[] = "input_params";
constexpr char OUTPUT_PARAMS[] = "output_params";

CBModule::CBModule(QObject* Parent)
    : QObject(Parent)
{

}

CBModule::~CBModule()
{

}

bool CBModule::ParseConfig(const QJsonDocument& JsonData)
{
    QJsonObject ConfigObject = JsonData.object();

    if (ConfigObject.contains(NAME)) {
        Name = ConfigObject.value(NAME).toString();
    }
    if (ConfigObject.contains(VERSION)) {
        Version = ConfigObject.value(VERSION).toString();
    }
    if (ConfigObject.contains(PATH)) {
        Path = ConfigObject.value(PATH).toString();
    }

    if (ConfigObject.contains(OUTPUT_PARAMS)) {
        QJsonArray OutputParams = ConfigObject.value(OUTPUT_PARAMS).toArray();
        for (const QJsonValueRef& Param : OutputParams) {
            if (QSharedPointer<CBModuleOutput> Output = CBModuleOutputFactory::CreateOutput(Param.toObject())) {
                Outputs.push_back(Output);
            }
        }
    }

    return !Name.isEmpty() || !Version.isEmpty() || !Path.isEmpty();
}

void CBModule::setSelected(bool bSelected)
{
    bIsSelected = bSelected;
    emit selectionChanged();
}

bool CBModule::IsSelected() const
{
    return bIsSelected;
}

const QString& CBModule::GetName() const
{
    return Name;
}

const QString& CBModule::GetVersion() const
{
    return Version;
}

const QString& CBModule::GetPath() const
{
    return Path;
}

int CBModule::getBindsCount() const
{
    return Binds.count();
}

int CBModule::getOutputsCount() const
{
    return Outputs.count();
}

CBModuleOutput* CBModule::getOutput(int Index)
{
    if (Index < 0 && Index >= Outputs.count()) {
        return nullptr;
    }

    QSharedPointer<CBModuleOutput> Output = Outputs[Index];

    if (!Output.isNull()) {
        QQmlEngine::setObjectOwnership(Output.data(), QQmlEngine::CppOwnership);
        return Output.data();
    }

    return nullptr;
}

void CBModule::bindNewAction(ActionsManager* Manager, const QString& ActionName)
{
    if (Manager == nullptr) {
        return;
    }

    QSharedPointer<Action> BindAction = Manager->GetActionByName(ActionName);
    if (BindAction.isNull()) {
        LOG_WARNING("Invalid action to bind");
        return;
    }

    for (const QSharedPointer<CBModuleBindConfig>& Bind : Binds) {
        if (Bind.isNull()) {
            continue;
        }

        if (Bind->IsBindedToAction(BindAction)) {
            LOG_WARNING("Action %s already binded", BindAction->getName().toStdString().c_str());
            return;
        }
    }

    if (QSharedPointer<CBModuleBindConfig> NewBind = CBModuleBindConfig::CreateBindConfig(BindAction, Outputs)) {
        Binds.push_back(NewBind);
    }

    emit bindsUpdated();
}

CBModuleBindConfig* CBModule::getBindConfig(int Index) const
{
    if (Index < 0 && Index >= Outputs.count()) {
        return nullptr;
    }

    QSharedPointer<CBModuleBindConfig> Output = Binds[Index];

    if (!Output.isNull()) {
        QQmlEngine::setObjectOwnership(Output.data(), QQmlEngine::CppOwnership);
        return Output.data();
    }

    return nullptr;
}

void CBModule::selectBind(int Index)
{
    if (Index < 0 && Index >= Outputs.count()) {
        return;
    }

    for (QSharedPointer<CBModuleBindConfig>& Config : Binds) {
        if (!Config.isNull() && Config->IsSelected()) {
            Config->setSelected(false);
        }
    }

    if (QSharedPointer<CBModuleBindConfig>& Config = Binds[Index]) {
        Config->setSelected(true);
    }
}

CBModuleBindConfig* CBModule::getSelectedBindConfig()
{
    for (QSharedPointer<CBModuleBindConfig>& Config : Binds) {
        if (!Config.isNull() && Config->IsSelected()) {
            QQmlEngine::setObjectOwnership(Config.data(), QQmlEngine::CppOwnership);
            return Config.data();
        }
    }

    return nullptr;
}

