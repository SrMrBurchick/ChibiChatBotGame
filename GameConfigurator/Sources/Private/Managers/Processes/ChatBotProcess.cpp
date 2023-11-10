#include "Managers/Processes/ChatBotProcess.h"

ChatBotProcess::ChatBotProcess()
    :IProcess(QT_STRINGIFY(CHAT_BOT_PATH), eProcessType::ChatBot)
{
}

ChatBotProcess::~ChatBotProcess()
{
}

void ChatBotProcess::Kill()
{
    Terminate();
}
