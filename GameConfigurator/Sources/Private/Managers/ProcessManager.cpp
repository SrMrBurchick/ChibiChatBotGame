#include "Managers/ProcessManager.h"
#include "Managers/NotificationsManager.h"
#include "Managers/Processes/GameProcess.h"

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

    // Connect signals
    QObject::connect(
        Process->GetProcess().get(), &QProcess::finished,
        [=]() {
            switch (Process->GetType()) {
                case eProcessType::Game:
                    emit gameEnded();
                    break;
                case eProcessType::Bot:
                    emit botEnded();
                    break;
            }
        }
    );

    QObject::connect(
        Process->GetProcess().get(), &QProcess::started,
        [=]() {
            switch (Process->GetType()) {
                case eProcessType::Game:
                    emit gameStarted();
                    break;
                case eProcessType::Bot:
                    emit botStarted();
                    break;
            }
        }
    );

    if (Process->GetType() == eProcessType::Game) {
        QObject::connect((GameProcess*)(Process.get()), &GameProcess::gameRunningAt, [=](QString GameInfo){
            emit this->gameRunningAt(GameInfo);
        });
    }

    ProcessesList.insert(Process->GetType(), Process);

    return true;
}

void ProcessManager::runBot()
{
    if (TryToRunProcess(eProcessType::Bot)) {
        NotificationsManager::SendNotification("Bot process", "Bot is running");
    } else {
        QString Error;
        QPointer<IProcess> Process = ProcessesList[eProcessType::Bot];
        if (!Process.isNull()) {
            Error = Process->GetLastError();
        }

        NotificationsManager::SendNotification("Bot process", QString::asprintf("Failed to run bot\n %s", Error.toStdString().c_str()));
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

bool ProcessManager::isBotRunning() const
{
    if (!ProcessesList.contains(eProcessType::Bot)) {
        NotificationsManager::SendNotification("Chat Bot not configured");
        return false;
    }

    return ProcessesList[eProcessType::Bot]->IsProcessRunning();
}

bool ProcessManager::isGameRunning() const
{
    if (!ProcessesList.contains(eProcessType::Game)) {
        NotificationsManager::SendNotification("Game not configured");
        return false;
    }

    return ProcessesList[eProcessType::Game]->IsProcessRunning();
}

void ProcessManager::stopBotRunning()
{
    if (!ProcessesList.contains(eProcessType::Bot)) {
        NotificationsManager::SendNotification("Chat Bot not configured");
        return;
    }

    ProcessesList[eProcessType::Bot]->Kill();
}

void ProcessManager::stopGameRunning()
{
    if (!ProcessesList.contains(eProcessType::Game)) {
        NotificationsManager::SendNotification("Game not configured");
        return;
    }

    ProcessesList[eProcessType::Game]->Kill();
}
