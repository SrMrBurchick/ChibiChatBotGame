#include "Managers/ProcessInterface.h"

IProcess::IProcess(const QString& _Program, const eProcessType _Type)
    :QObject(), Program(_Program), Type(_Type)
{
}

IProcess::~IProcess()
{
    Kill();
}

bool IProcess::RunProcessWithParams(const QStringList& Args)
{
    if (IsProcessRunning()) {
        return true;
    }

    if (Program.isEmpty()) {
        return false;
    }

    Process.start(Program, Args);

    return IsProcessRunning();
}

bool IProcess::RunProcess()
{
    if (IsProcessRunning()) {
        return true;
    }

    if (Program.isEmpty()) {
        return false;
    }

    Process.start(Program);

    return IsProcessRunning();
}

bool IProcess::StopProcess()
{
    if (!IsProcessRunning()) {
        return true;
    }

    Process.close();

    return !IsProcessRunning();
}

bool IProcess::IsProcessRunning()
{
    return Process.state() == QProcess::Running;
}

void IProcess::Kill()
{
    if (IsProcessRunning()) {
        Process.kill();
    }
}
