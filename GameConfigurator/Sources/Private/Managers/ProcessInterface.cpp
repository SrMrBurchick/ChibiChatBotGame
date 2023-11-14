#include "Managers/ProcessInterface.h"
#include "Managers/NotificationsManager.h"
#include "System/Logger.h"

#include <QString>

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
    LOG_INFO("Running a program: %s, with params: %s", Program.toStdString().c_str(), Args.join(" ").toStdString().c_str());

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
    LOG_INFO("Running a program: %s", Program.toStdString().c_str());
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
    LOG_INFO("Stop a program: %s", Program.toStdString().c_str());

    return !IsProcessRunning();
}

bool IProcess::IsProcessRunning()
{
    return Process.state() != QProcess::NotRunning;
}

void IProcess::Kill()
{
    NotificationsManager::SendNotification("Process manager", QString::asprintf("Kill a\n %s", Program.toStdString().c_str()));
    LOG_INFO("Kill a program: %s", Program.toStdString().c_str());
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

void IProcess::Terminate()
{
    NotificationsManager::SendNotification("Process manager", QString::asprintf("Kill a\n [%lld] %s", Process.processId(), Program.toStdString().c_str()));
    LOG_INFO("Terminate a program: %s", Program.toStdString().c_str());

#ifdef Q_OS_WIN
    QString KillCommand = QString("taskkill.exe");
    QStringList Args = {"/F", "/T", "/PID", QString::number(Process.processId())};
    QProcess::execute(KillCommand, Args);
#else
    Process.terminate();
#endif

}

int IProcess::GetProcessId() const
{
    return Process.processId();
}
