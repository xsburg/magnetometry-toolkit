#include "Logger.h"

#include <iostream>

namespace common
{
    void Logger::initialize(LogLevel logLevel)
    {
        if (logLevel < 0)
        {
            logLevel = Debug;
        }
        this->logLevel = logLevel;
    }

    void Logger::debug(const QString& message)
    {
        write(Debug, message);
    }

    void Logger::trace(const QString& message)
    {
        write(Trace, message);
    }

    void Logger::info(const QString& message)
    {
        write(Info, message);
    }

    void Logger::warn(const QString& message)
    {
        write(Warn, message);
    }

    void Logger::error(const QString& message)
    {
        write(Error, message);
    }

    void Logger::fatal(const QString& message)
    {
        write(Fatal, message);
    }

    void Logger::write(LogLevel logLevel, const QString& message)
    {
        if (this->logLevel >= logLevel)
        {
            QString levelMsg;
            switch (logLevel)
            {
            case Debug:
                levelMsg = "DEBUG";
                break;
            case Trace:
                levelMsg = "TRACE";
                break;
            case Info:
                levelMsg = "INFO";
                break;
            case Warn:
                levelMsg = "WARN";
                break;
            case Error:
                levelMsg = "ERROR";
                break;
            case Fatal:
                levelMsg = "FATAL";
                break;
            default:
                break;
            }
            std::cerr << levelMsg.toStdString() << ": " << message.toStdString() << std::endl;
        }
    }
}
