#include "Managers/ProcessManager.h"
#include "Managers/NotificationsManager.h"
#include "Managers/Processes/GameProcess.h"
#include "Managers/Processes/BotProcess.h"

#include "System/Logger.h"

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
    SetBusy(true);
    if (!ProcessesList.contains(Process)) {
        return false;
    }

    return ProcessesList[Process]->RunProcess();
}

bool ProcessManager::TryToStopProcess(eProcessType Process)
{
    SetBusy(true);
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
    SetBusy(true);
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
            SetBusy(false);
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

    if (Process->GetType() == eProcessType::Bot) {
        QObject::connect((BotProcess*)(Process.get()), &BotProcess::botStatusChanged, [this](int Status, const QString& Type){
            LOG_INFO("Received chatbot status = %d, type = %s", Status, Type.toStdString().c_str());
            SetBusy(false);
            if (Status != 202) {
                NotificationsManager::SendNotification("Bot process", QString::asprintf("Failed to subscribe!\n%s", Type.toStdString().c_str()));
                stopBotRunning();
            } else {
                NotificationsManager::SendNotification("Bot process", QString::asprintf("Successfully subscribed!\n%s", Type.toStdString().c_str()));
            }
        });
    }

    if (Process->GetType() == eProcessType::Game) {
        QObject::connect((GameProcess*)(Process.get()), &GameProcess::gameRunningAt, [=](QString GameInfo){
            SetBusy(false);
            emit this->gameRunningAt(GameInfo);
        });
    }

    ProcessesList.insert(Process->GetType(), Process);

    return true;
}

void ProcessManager::runBot()
{
    SetBusy(true);
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
    SetBusy(true);
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

void ProcessManager::SetBusy(bool isBusy)
{
    bIsBusy = isBusy;
    emit busyUpdated();
}
