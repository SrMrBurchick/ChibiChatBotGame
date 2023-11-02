#include "Managers/Processes/GameProcess.h"

GameProcess::GameProcess()
    :IProcess(QT_STRINGIFY(WASM_SERVER_RUNNER), eProcessType::Game)
{
}

GameProcess::~GameProcess()
{

}

bool GameProcess::RunProcess()
{
    return IProcess::RunProcessWithParams(QStringList(QT_STRINGIFY(GAME_PATH)));
}
