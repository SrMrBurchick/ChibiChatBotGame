#include "Managers/ProcessManager.h"
#include "Managers/NotificationsManager.h"

ProcessManager::ProcessManager(QObject* Parent)
    :QObject(Parent)
{
}

ProcessManager::~ProcessManager()
{
    StopAll();
    ProcessesList.clear();
}

bool ProcessManager::TryToRunProcess(eProcessType Process)
{
    if (!ProcessesList.contains(Process)) {
        return false;
    }

    return ProcessesList[Process]->RunProcess();
}

bool ProcessManager::TryToStopProcess(eProcessType Process)
{
    if (!ProcessesList.contains(Process)) {
        return false;
    }

    return ProcessesList[Process]->StopProcess();
}

void ProcessManager::ForceStopProcess(eProcessType Process)
{
    if (ProcessesList.contains(Process)) {
        ProcessesList[Process]->Kill();
    }
}

void ProcessManager::StopAll()
{
    for (QPointer<IProcess> Process : ProcessesList) {
        Process->Kill();
    }
}

bool ProcessManager::AddProcess(QPointer<IProcess> Process)
{
    if (Process == nullptr) {
        return false;
    }

    if (ProcessesList.contains(Process->GetType())) {
        return false;
    }

    ProcessesList.insert(Process->GetType(), Process);

    return true;
}

void ProcessManager::runChatBot()
{
    if (TryToRunProcess(eProcessType::ChatBot)) {
        NotificationsManager::SendNotification("Chat bot process", "Chat bot is running");
    } else {
        QString Error;
        QPointer<IProcess> Process = ProcessesList[eProcessType::ChatBot];
        if (!Process.isNull()) {
            Error = Process->GetLastError();
        }

        NotificationsManager::SendNotification("Chat bot process", QString::asprintf("Failed to run chat bot\n %s", Error.toStdString().c_str()));
    }
}

void ProcessManager::runGame()
{
    if (TryToRunProcess(eProcessType::Game)) {
        NotificationsManager::SendNotification("Game process", "Game running");
    } else {
        QString Error;
        QPointer<IProcess> Process = ProcessesList[eProcessType::Game];
        if (!Process.isNull()) {
            Error = Process->GetLastError();
        }

        NotificationsManager::SendNotification("Game process", QString::asprintf("Failed to run game\n %s", Error.toStdString().c_str()));
    }
}

void ProcessManager::killAll()
{
    StopAll();
}

bool ProcessManager::isChatBotRunning() const
{
    if (!ProcessesList.contains(eProcessType::ChatBot)) {
        NotificationsManager::SendNotification("Chat Bot not configured");
        return false;
    }

    return ProcessesList[eProcessType::ChatBot]->IsProcessRunning();
}

bool ProcessManager::isGameRunning() const
{
    if (!ProcessesList.contains(eProcessType::Game)) {
        NotificationsManager::SendNotification("Game not configured");
        return false;
    }

    return ProcessesList[eProcessType::Game]->IsProcessRunning();
}

void ProcessManager::stopChatBotRunning()
{
    if (!ProcessesList.contains(eProcessType::ChatBot)) {
        NotificationsManager::SendNotification("Chat Bot not configured");
        return;
    }

    ProcessesList[eProcessType::ChatBot]->Kill();
}

void ProcessManager::stopGameRunning()
{
    if (!ProcessesList.contains(eProcessType::Game)) {
        NotificationsManager::SendNotification("Game not configured");
        return;
    }

    ProcessesList[eProcessType::Game]->Kill();
}
