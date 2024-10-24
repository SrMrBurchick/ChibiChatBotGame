#pragma once

#include <QProcess>
#include <QObject>

enum eProcessType {
    Game,
    Bot
};

class IProcess: public QObject {
    Q_OBJECT

public:
    explicit IProcess(const QString& Program, const eProcessType Type);
    virtual ~IProcess();

    virtual bool RunProcess();
    virtual bool StopProcess();
    virtual bool RestartProcess();

    bool RunProcessWithParams(const QStringList& Args, QProcessEnvironment* Env = nullptr);
    bool IsProcessRunning();

    virtual void Kill();
    void Terminate();

    eProcessType GetType() const {return Type;}
    QString GetLastError();

    QPointer<QProcess> GetProcess();

    int GetProcessId() const;

private:
    QProcess Process;
    QString Program;
    eProcessType Type;
};
