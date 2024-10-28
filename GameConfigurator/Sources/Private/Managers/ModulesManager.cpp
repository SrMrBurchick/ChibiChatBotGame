#include "Managers/ModulesManger.h"

#include "Managers/NotificationsManager.h"
#include "Core/Modules/Module.h"
#include "Configuration/ConfigObject.h"

#include "System/Logger.h"

#include <QFile>
#include <QDir>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QQmlEngine>

constexpr char CONFIG_FILE_NAME [] = "ModuleConfig";

CBModulesManager::CBModulesManager(QObject* Parent)
    : QObject(Parent)
{

}

CBModulesManager::~CBModulesManager()
{
}

void CBModulesManager::SetBusy(bool isBusy)
{
    bIsBusy = isBusy;
    emit busyUpdated();
}

void CBModulesManager::initModules()
{
    SetBusy(true);
    SearchModules();
}

bool CBModulesManager::IsModuleExists(const QSharedPointer<CBModule>& NewModule) const
{
    for (const QSharedPointer<CBModule>& Module : Modules) {
        if (!Module.isNull() && !NewModule.isNull()) {
            if ((Module->GetName() == NewModule->GetName()) && (Module->GetPath() == NewModule->GetPath())) {
                return true;
            }
        }
    }

    return false;
}

void CBModulesManager::SearchModules()
{
    QList<QFileInfo> ModulesConfig = GetModules();

    if (ModulesConfig.isEmpty()) {
        SetBusy(false);
        NotificationsManager::SendNotification("Modules Manager", "No modules found!");
        return;
    }

    for (const QFileInfo& FileInfo : ModulesConfig) {
        QFile ConfigFile(FileInfo.filePath());
        if (ConfigFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
            QJsonDocument JsonConfig = QJsonDocument::fromJson(ConfigFile.readAll());
            if (JsonConfig.isNull() || JsonConfig.isEmpty()) {
                continue;
            }

            if (QSharedPointer<CBModule> NewModule = QSharedPointer<CBModule>::create()) {
                if (NewModule->ParseConfig(JsonConfig) && !IsModuleExists(NewModule)) {
                    Modules.push_back(NewModule);
                    NotificationsManager::SendNotification("Modules Manager", "Added New Module");
                }
            }
        }
    }

    if (!bInitialized) {
        bInitialized = true;
        emit initialized();
    }

    emit modulesSynced();
    SetBusy(false);
}

QList<QFileInfo> CBModulesManager::GetModules()
{
    QList<QFileInfo> ModulesConfig;
    LOG_INFO("Search for modules in: %s", QT_STRINGIFY(MODULES_FOLDER));
    QDir Directory(QT_STRINGIFY(MODULES_FOLDER));
    for (const QFileInfo& FileInfo : Directory.entryInfoList(QDir::AllDirs | QDir::NoDotAndDotDot | QDir::NoSymLinks)) {
        if (FileInfo.isDir()) {
            QFileInfo Result = SearchModuleConfig(FileInfo.absoluteFilePath());
            if (Result.exists()) {
                ModulesConfig.push_back(Result);
            }
        }
    }

    LOG_INFO("Modules count : %d", ModulesConfig.count());
    return ModulesConfig;
}

QFileInfo CBModulesManager::SearchModuleConfig(const QDir& Directory)
{
    QStringList ConfigExtension = {"*.json"};
    if (Directory.exists()) {
        for (const QFileInfo& FileInfo : Directory.entryInfoList(ConfigExtension, QDir::Files | QDir::NoDotAndDotDot | QDir::NoSymLinks)) {
            if (FileInfo.baseName() == CONFIG_FILE_NAME) {
                return FileInfo;
            }
        }

        for (const QFileInfo& FileInfo : Directory.entryInfoList(ConfigExtension, QDir::AllDirs | QDir::NoDotAndDotDot | QDir::NoSymLinks)) {
            QFileInfo Result = SearchModuleConfig(FileInfo.absoluteFilePath());
            if (Result.exists()) {
                return Result;
            }
        }
    }

    return QFileInfo();
}

void CBModulesManager::selectModule(int Index)
{
    LOG_INFO("Try to select %d module", Index);
    if (Index < 0 && Index >= Modules.length()) {
        return;
    }
    for (QSharedPointer<CBModule>& Module : Modules) {
        if (!Module.isNull() && Module->IsSelected()) {
            LOG_INFO("Turn off selection from %s", Module->GetName().toStdString().c_str());
            Module->setSelected(false);
        }
    }

    QSharedPointer<CBModule> SelectedModule = Modules[Index];
    if (!SelectedModule.isNull()) {
        LOG_INFO("Turn on selection for %s", SelectedModule->GetName().toStdString().c_str());
        SelectedModule->setSelected(true);
    }

    LOG_INFO("Module %s Selected", SelectedModule->GetName().toStdString().c_str());
    emit moduleSelected();
}

CBModule* CBModulesManager::getSelectedModule() const
{
    for (const QSharedPointer<CBModule>& Module : Modules) {
        if (!Module.isNull() && Module->IsSelected()) {
            QQmlEngine::setObjectOwnership(Module.data(), QQmlEngine::CppOwnership);
            return Module.data();
        }
    }

    return nullptr;
}

CBModule* CBModulesManager::getModule(int Index) const
{
    if (Index < 0 && Index >= Modules.length()) {
        return nullptr;
    }

    QSharedPointer<CBModule> Module = Modules[Index];
    if (!Module.isNull()) {
        QQmlEngine::setObjectOwnership(Module.data(), QQmlEngine::CppOwnership);
    }

    return Module.data();
}

int CBModulesManager::getModulesCount() const
{
    return Modules.count();
}

void CBModulesManager::saveConfig(ConfigObject* Config)
{
    SetBusy(true);
    QJsonArray ModulesConfig;
    for (const QSharedPointer<CBModule>& Module : Modules) {
        if (!Module.isNull() && (Module->getBindsCount() > 0)) {
            QJsonObject ModuleConfig = Module->GenerateConfig();
            if (!ModuleConfig.isEmpty()) {
                ModulesConfig.push_back(ModuleConfig);
            }
        }
    }

    if (Config && !ModulesConfig.isEmpty()) {
        Config->SaveModules(ModulesConfig);
        Config->saveConfig();
    }

    SetBusy(false);
}

void CBModulesManager::parseConfig(ConfigObject* Config)
{
    if (!Config) {
        return;
    }

    SetBusy(true);

    for (QJsonValueConstRef Value : Config->GetModulesConfig()) {
        for (QSharedPointer<CBModule>& Module : Modules) {
            if (!Module.isNull()) {
                Module->ParseGameConfig(Value.toObject());
            }
        }
    }

    SetBusy(false);
}
