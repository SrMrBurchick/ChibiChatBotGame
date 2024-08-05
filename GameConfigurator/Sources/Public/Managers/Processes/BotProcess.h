#pragma once

#include "Managers/ProcessInterface.h"

class BotProcess : public IProcess {
public:
    BotProcess();
    virtual ~BotProcess();

    virtual void Kill();
};
