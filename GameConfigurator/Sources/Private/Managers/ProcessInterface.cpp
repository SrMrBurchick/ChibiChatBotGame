#include "Managers/ProcessInterface.h"
#include "Managers/NotificationsManager.h"

#include <QString>
#include <iostream>

IProcess::IProcess(const QString& _Program, const eProcessType _Type)
    :QObject(), Program(_Program), Type(_Type)
{
}

IProcess::~IProcess()
{
    Kill();
}

bool IProcess::RunProcessWithParams(const QStringList& Args, QProcessEnvironment* Env)
{
    if (IsProcessRunning()) {
        return true;
    }

    if (Program.isEmpty()) {
        return false;
    }

    if (nullptr != Env) {
        Process.setProcessEnvironment(*Env);
    }

    Process.start(Program, Args);

    NotificationsManager::SendNotification("Process manager", QString::asprintf("Running a\n %s", Program.toStdString().c_str()));

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

    NotificationsManager::SendNotification("Process manager", QString::asprintf("Running a\n %s", Program.toStdString().c_str()));
    Process.start(Program);

    return Process.isOpen();
}

bool IProcess::StopProcess()
{
    if (!IsProcessRunning()) {
        return true;
    }

    Process.close();

    NotificationsManager::SendNotification("Process manager", QString::asprintf("Stop a\n %s", Program.toStdString().c_str()));

    return !IsProcessRunning();
}

bool IProcess::IsProcessRunning()
{
    return Process.state() != QProcess::NotRunning;
}

void IProcess::Kill()
{
    NotificationsManager::SendNotification("Process manager", QString::asprintf("Kill a\n %s", Program.toStdString().c_str()));
    Process.kill();
}

QString IProcess::GetLastError()
{
    return Process.readAllStandardError();
}

bool IProcess::RestartProcess()
{
    return Process.reset();
}

QPointer<QProcess> IProcess::GetProcess()
{
    return QPointer<QProcess>(&Process);
}
