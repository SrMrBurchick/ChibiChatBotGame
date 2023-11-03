#include "Managers/Processes/GameProcess.h"

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
    QString EnvironmentWasmDir(std::getenv("WASM_SERVER_RUNNER_DIRECTORY"));
    QString GamePath(QT_STRINGIFY(GAME_PATH));
    if (EnvironmentWasmDir != GamePath) {
        setenv(
            "WASM_SERVER_RUNNER_DIRECTORY",
            GamePath.toStdString().c_str(),
            1
        );
    }

    return IProcess::RunProcessWithParams(QStringList(QT_STRINGIFY(GAME)));
}
