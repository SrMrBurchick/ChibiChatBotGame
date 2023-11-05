#include "Managers/Processes/GameProcess.h"

#include <QProcessEnvironment>
#include <cstdlib>

GameProcess::GameProcess()
    :IProcess(QT_STRINGIFY(WASM_SERVER_RUNNER), eProcessType::Game)
{
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
