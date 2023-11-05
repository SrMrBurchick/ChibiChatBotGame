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
    Q_INVOKABLE void killAll();
    Q_INVOKABLE bool isChatBotRunning() const;
    Q_INVOKABLE bool isGameRunning() const;
    Q_INVOKABLE void stopChatBotRunning();
    Q_INVOKABLE void stopGameRunning();

signals:
    void gameStarted();
    void gameEnded();
    void chatBotStarted();
    void chatBotEnded();

private:
    QMap<eProcessType, QPointer<IProcess>> ProcessesList;
};
