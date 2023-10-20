#include "Managers/Processes/GameProcess.h"

GameProcess::GameProcess()
    :IProcess(QT_STRINGIFY(WASM_RUNNER_PATH), eProcessType::Game)
{
}

GameProcess::~GameProcess()
{

}

bool GameProcess::RunProcess()
{
    QStringList Arguments;
    Arguments.push_back(QT_STRINGIFY(GAME_PATH));
    return RunProcessWithParams(Arguments);
}
