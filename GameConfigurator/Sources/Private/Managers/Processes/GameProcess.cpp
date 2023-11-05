#include "Managers/Processes/GameProcess.h"
#include "Managers/NotificationsManager.h"

#include <QProcessEnvironment>
#include <QRegularExpression>
#include <cstdlib>

GameProcess::GameProcess()
    :IProcess(QT_STRINGIFY(WASM_SERVER_RUNNER), eProcessType::Game)
{
    QObject::connect(
        GetProcess(), &QProcess::readyReadStandardOutput,
        this, &GameProcess::ReadyGetGameOutput
    );
}

GameProcess::~GameProcess()
{

}

bool GameProcess::RunProcess()
{
    QProcessEnvironment Environment = QProcessEnvironment::systemEnvironment();
    QString EnvironmentWasmDir(std::getenv("WASM_SERVER_RUNNER_DIRECTORY"));
    QString GamePath(QT_STRINGIFY(GAME_PATH));
    if (EnvironmentWasmDir != GamePath) {
        Environment.insert("WASM_SERVER_RUNNER_DIRECTORY", GamePath);
    }

    return IProcess::RunProcessWithParams(QStringList(QT_STRINGIFY(GAME)), &Environment);
}

void GameProcess::ReadyGetGameOutput()
{
    QPointer<QProcess> Process = GetProcess();
    if (Process.isNull()) {
        return;
    }

    QString Output = QString::fromUtf8(Process->readAllStandardOutput());
    QRegularExpression pattern("http://([\\w.]+):(\\d+)");
    QRegularExpressionMatch match = pattern.match(Output);

    if (match.hasMatch()) {
        NotificationsManager::SendNotification(
            "Game Info",
            QString("Game running at: %1").arg(match.captured())
        );

        emit gameRunningAt(match.captured());
    }
}
