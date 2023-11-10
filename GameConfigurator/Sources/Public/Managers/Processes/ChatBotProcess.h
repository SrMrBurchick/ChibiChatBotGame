#pragma once

#include "Managers/ProcessInterface.h"

class ChatBotProcess : public IProcess {
public:
    ChatBotProcess();
    virtual ~ChatBotProcess();

    virtual void Kill();
};
