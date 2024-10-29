#include "Managers/Processes/BotProcess.h"
#include "System/Logger.h"


#include <QPointer>
#include <QProcessEnvironment>
#include <QRegularExpression>
#include <cstdlib>

BotProcess::BotProcess()
    :IProcess(QT_STRINGIFY(TWITCH_BOT_PATH), eProcessType::Bot)
{
    QObject::connect(
        GetProcess(), &QProcess::readyReadStandardOutput,
        this, &BotProcess::ReadyGetOutput
    );
}

BotProcess::~BotProcess()
{
}


bool BotProcess::RunProcess()
{
    QProcessEnvironment Environment = QProcessEnvironment::systemEnvironment();
    Environment.insert("CLIENT_ID", QT_STRINGIFY(CLIENT_ID));

    return IProcess::RunProcessWithParams(QStringList(), &Environment);
}

void BotProcess::Kill()
{
    Terminate();
}

void BotProcess::ReadyGetOutput()
{
    QPointer<QProcess> Process = GetProcess();
    if (Process.isNull()) {
        return;
    }

    int Status = -1;
    QString Type;

    QString Output = QString::fromUtf8(Process->readAllStandardOutput());
    LOG_INFO("[TWITCH_BOT] %s", Output.toStdString().c_str());
    QRegularExpression pattern(R"(TWITCH_SUB_STATUS=(\d+))");
    QRegularExpressionMatch match = pattern.match(Output);

    if (match.hasMatch()) {
        LOG_INFO("Match = %s", match.captured(1).toStdString().c_str());
        Status = match.captured(1).toInt();
    }

    pattern = QRegularExpression("TWITCH_SUB_TYPE=\"([^\"]+)\"");
    match = pattern.match(Output);

    if (match.hasMatch()) {
        LOG_INFO("Match = %s", match.captured(1).toStdString().c_str());
        Type = match.captured(1);
    }

    if (Status != -1) {
        emit botStatusChanged(Status, Type);
    }
}
