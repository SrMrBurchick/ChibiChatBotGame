#include "System/Logger.h"
#include <QTextStream>
#include <iostream>
#include <csignal>
#include <cerrno>

Logger::~Logger()
{
    LogFile.close();
}

QString Logger::LogLevelToString(LogLevel Level)
{
    QString StringLevel = "[INFO] ";

    switch (Level) {
        case LogLevel::WARNING:
            StringLevel = "[WARNING] ";
            break;
        case LogLevel::CRITICAL:
            StringLevel = "[CRITICAL] ";
            break;
        default:
            break;
    }

    return StringLevel;
}

void Logger::LogToFile(const QString& LogMsg)
{
    QTextStream FileStream(&LogFile);
    FileStream << LogMsg << Qt::endl;
    FileStream.flush();
}

void Logger::LogToConsole(const QString& LogMsg)
{
    std::cout << LogMsg.toStdString().c_str() << std::endl;
}

void Logger::InitLogger(const QString& LoggerFile)
{
    Logger& Log = Logger::GetLogger();

    if (!Log.bIsEnabled) {
        return;
    }

    Log.LogFile.setFileName(LoggerFile);
    Log.LogFile.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text);
    signal(SIGSEGV, Logger::SignalHandler);
}

void Logger::SignalHandler(int Signal)
{
    // Obtain the last standard error message
    const char* lastError = strerror(errno);

    LOG_CRITICAL("Signal {%d} received! Error :%s", Signal, lastError);
    std::signal(Signal, SIG_DFL);
    std::raise(Signal);
}

void Logger::SetLoggerEnabled(bool Enabled)
{
    Logger& Log = Logger::GetLogger();
    Log.bIsEnabled = Enabled;

    std::cout << "Set logger enabled to :" << Enabled << std::endl;
    if (!Log.bIsEnabled) {
        Log.LogFile.close();
    } else {
        InitLogger("chibi_log.txt");
    }
}
