#include "Core/Modules/Binds/ModuleBindConfig.h"
#include "Core/Modules/Outputs/ModuleOutput.h"
#include "Core/Modules/Binds/ModuleBindResultConfig.h"

#include "Core/Action.h"
#include "System/Logger.h"

#include <QQmlEngine>
#include <QJsonObject>
#include <QJsonArray>

constexpr char TARGET_ACTION[] = "target_action";
constexpr char RESULTS[] = "results";

CBModuleBindConfig::CBModuleBindConfig(QObject* Parent)
    : QObject(Parent)
{

}

CBModuleBindConfig::~CBModuleBindConfig()
{
    if (!TargetAction.isNull()) {
        LOG_INFO("Remove Module Bind config for action = %s", TargetAction->getName().toStdString().c_str());
    } else {
        LOG_INFO("Remove Module Bind config");
    }
}

bool CBModuleBindConfig::IsBindedToAction(QSharedPointer<Action> NewAction) const
{
    if (!NewAction.isNull() && !TargetAction.isNull()) {
        return NewAction->getName() == TargetAction->getName();
    }

    return false;
}

QSharedPointer<CBModuleBindConfig> CBModuleBindConfig::CreateBindConfig(const QSharedPointer<Action>& NewAction, const QVector<QSharedPointer<CBModuleOutput>>& Outputs)
{
    if (QSharedPointer<CBModuleBindConfig> NewConfig = QSharedPointer<CBModuleBindConfig>::create()) {
        if (!NewAction.isNull()) {
            NewConfig->TargetAction = NewAction;
        }

        for (const QSharedPointer<CBModuleOutput>& Output : Outputs) {
            if (QSharedPointer<CBModuleBindResultConfig> Result = CBModuleBindResultConfig::CreateResultConfig(Output)) {
                LOG_INFO("Add new result config for output = %s", Output->GetName().toStdString().c_str());
                NewConfig->Configs.push_back(Result);
            }
        }

        return NewConfig;
    }

    return nullptr;
}

Action* CBModuleBindConfig::getTargetAction() const
{
    if (!TargetAction.isNull()) {
        QQmlEngine::setObjectOwnership(TargetAction.data(), QQmlEngine::CppOwnership);
        return TargetAction.data();
    }

    return nullptr;
}

void CBModuleBindConfig::setSelected(bool bSelected)
{
    bIsSelected = bSelected;
    emit selectionChanged();
}

bool CBModuleBindConfig::IsSelected() const
{
    return bIsSelected;
}

CBModuleBindResultConfig* CBModuleBindConfig::getResultConfig(int Index) const
{
    if (Index < 0 && Index >= Configs.count()) {
        return nullptr;
    }

    if (QSharedPointer<CBModuleBindResultConfig> Config = Configs[Index]) {
        QQmlEngine::setObjectOwnership(Config.data(), QQmlEngine::CppOwnership);
        return Config.data();
    }

    return nullptr;
}

int CBModuleBindConfig::getResultsConfigCount() const
{
    return Configs.count();
}

QJsonObject CBModuleBindConfig::GenerateConfig() const
{
    QJsonObject ConfigJSON;
    QJsonArray Results;

    for (const QSharedPointer<CBModuleBindResultConfig>& Config : Configs) {
        if (!Config.isNull()) {
            QJsonObject BindConfig = Config->GenerateConfig();
            if (!BindConfig.isEmpty()) {
                Results.push_back(BindConfig);
            }
        }
    }

    if (!TargetAction.isNull() && !Results.isEmpty()) {
        ConfigJSON.insert(TARGET_ACTION, TargetAction->getName());
        ConfigJSON.insert(RESULTS, Results);
    }

    return ConfigJSON;
}

QString CBModuleBindConfig::GetTargetActionFromConfig(const QJsonObject& Config)
{
    if (Config.contains(TARGET_ACTION)) {
        return Config[TARGET_ACTION].toString();
    }

    return "";
}

bool CBModuleBindConfig::ParseConfig(const QJsonObject& Config)
{
    if (Config.contains(RESULTS)) {
        for (QJsonValueConstRef Value : Config[RESULTS].toArray()) {
            for (const QSharedPointer<CBModuleBindResultConfig>& Config : Configs) {
                if (!Config.isNull() && Config->HasTargetOutputFromConfig(Value.toObject())) {
                    Config->ParseConfig(Value.toObject());
                }
            }
        }
    }

    return false;
}
