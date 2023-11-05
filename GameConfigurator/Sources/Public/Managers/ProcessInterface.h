#pragma once

#include <QProcess>
#include <QObject>

enum eProcessType {
    Game,
    ChatBot
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

    void Kill();

    eProcessType GetType() const {return Type;}
    QString GetLastError();

private:
    QProcess Process;
    QString Program;
    eProcessType Type;
};
