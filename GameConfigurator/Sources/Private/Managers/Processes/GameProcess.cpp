#include "Managers/Processes/GameProcess.h"

GameProcess::GameProcess()
    :IProcess(QT_STRINGIFY(GAME_RUNNER_PATH), eProcessType::Game)
{
}

GameProcess::~GameProcess()
{

}
