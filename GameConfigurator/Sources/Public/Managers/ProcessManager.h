#pragma once
#include <QObject>
#include <QMap>
#include <QPointer>

#include "Managers/ProcessInterface.h"

class ProcessManager : public QObject {
    Q_OBJECT
public:
    explicit ProcessManager(QObject* Parent = nullptr);
    virtual ~ProcessManager();

    bool TryToRunProcess(eProcessType Process);
    bool TryToStopProcess(eProcessType Process);

    void ForceStopProcess(eProcessType Process);
    void StopAll();

    bool AddProcess(QPointer<IProcess> Process);

    Q_INVOKABLE void runGame();
    Q_INVOKABLE void runChatBot();

private:
    QMap<eProcessType, QPointer<IProcess>> ProcessesList;
};