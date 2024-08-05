#include "Managers/Processes/BotProcess.h"

BotProcess::BotProcess()
    :IProcess(QT_STRINGIFY(TWITCH_BOT_PATH), eProcessType::Bot)
{
}

BotProcess::~BotProcess()
{
}

void BotProcess::Kill()
{
    Terminate();
}
