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
    Q_INVOKABLE void runBot();
    Q_INVOKABLE void killAll();
    Q_INVOKABLE bool isBotRunning() const;
    Q_INVOKABLE bool isGameRunning() const;
    Q_INVOKABLE void stopBotRunning();
    Q_INVOKABLE void stopGameRunning();

signals:
    void gameStarted();
    void gameEnded();
    void botStarted();
    void botEnded();
    void gameRunningAt(QString Address);

private:
    QMap<eProcessType, QPointer<IProcess>> ProcessesList;
};
