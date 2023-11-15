#pragma once
#include <QFile>
#include <QTime>
#include <QString>

enum LogLevel {
    INFO = 0,
    WARNING,
    CRITICAL
};


#define LOG_INFO(format, ...) Logger::log(LogLevel::INFO, "[%s:%s:%d] " format, __FILE__, __FUNCTION__, __LINE__, ##__VA_ARGS__);
#define LOG_WARNING(format, ...) Logger::log(LogLevel::WARNING, "[%s:%s:%d] " format, __FILE__, __FUNCTION__, __LINE__, ##__VA_ARGS__);
#define LOG_CRITICAL(format, ...) Logger::log(LogLevel::CRITICAL, "[%s:%s:%d] " format, __FILE__, __FUNCTION__, __LINE__, ##__VA_ARGS__);

class Logger {
public:
    ~Logger();

    template <typename... Args>
    static void log(LogLevel Level, const char* format, Args&&... args)
    {
        if (!GetLogger().bIsEnabled) {
            return;
        }

        QString logMessage = formatLogMessage(Level, format, std::forward<Args>(args)...);
        GetLogger().LogToConsole(logMessage);
        GetLogger().LogToFile(logMessage);
    }

    // Format log message with timestamp and variadic parameters
    template <typename... Args>
    static QString formatLogMessage(LogLevel Level, const char* format, Args&&... args)
    {
        QString logMessage;

        logMessage += LogLevelToString(Level);
        logMessage += QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss") + " ";
        logMessage += QString::asprintf(format, std::forward<Args>(args)...);

        return logMessage;
    }

    static QString LogLevelToString(LogLevel Level);

    static void InitLogger(const QString& LoggerFile);

    static void SignalHandler(int Signal);

    static void SetLoggerEnabled(bool Enabled);

private:
    static Logger& GetLogger() {
        static Logger LoggerInst;
        return LoggerInst;
    }

    void LogToFile(const QString& LogMsg);
    void LogToConsole(const QString& LogMsg);

    QFile LogFile;
    bool bIsEnabled = false;
};
