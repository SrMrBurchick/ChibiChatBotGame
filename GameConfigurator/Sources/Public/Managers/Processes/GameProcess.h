#pragma once

#include "Managers/ProcessInterface.h"

class GameProcess: public IProcess {
public:
    GameProcess();
    virtual ~GameProcess();
    virtual bool RunProcess();

signals:
    void gameRunningAt(QString Address);

private slots:
    void ReadyGetGameOutput();
};
