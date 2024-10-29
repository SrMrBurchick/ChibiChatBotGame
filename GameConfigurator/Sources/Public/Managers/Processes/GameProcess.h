#pragma once

#include "Managers/ProcessInterface.h"

class GameProcess: public IProcess {
    Q_OBJECT
public:
    GameProcess();
    virtual ~GameProcess();
    virtual bool RunProcess() override;

signals:
    void gameRunningAt(QString Address);

private slots:
    void ReadyGetGameOutput();
};
