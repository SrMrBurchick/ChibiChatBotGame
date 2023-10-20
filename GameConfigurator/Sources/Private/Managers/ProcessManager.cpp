#include "Managers/ProcessManager.h"

ProcessManager::ProcessManager(QObject* Parent)
    :QObject(Parent)
{
}

ProcessManager::~ProcessManager()
{
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
        // TODO: notify success
    } else {
        // TODO notify fault
    }
}

void ProcessManager::runGame()
{
    if (TryToRunProcess(eProcessType::Game)) {
        // TODO: notify success
    } else {
        // TODO notify fault
    }
}
