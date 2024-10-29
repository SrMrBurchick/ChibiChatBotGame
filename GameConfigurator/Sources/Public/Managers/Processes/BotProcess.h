#pragma once

#include "Managers/ProcessInterface.h"

class BotProcess : public IProcess {
    Q_OBJECT

public:
    BotProcess();
    virtual ~BotProcess();

    virtual void Kill() override;
    virtual bool RunProcess() override;

signals:
    void botStatusChanged(int Status, const QString& Type);

private slots:
    void ReadyGetOutput();

};
