#include "Logger.h"

#include <QtCore/QtCore>
#include <iostream>

namespace Common
{
    void Logger::Initialize(LogLevel logLevel)
    {
        if (logLevel < 0)
        {
            logLevel = LogLevel::Debug;
        }

        this->logLevel = logLevel;
    }

    void Logger::Debug(const QString& message)
    {
        if (this->logLevel >= LogLevel::Debug)
        {
            std::cerr << message.toStdString() << std::endl;
        }
    }

    void Logger::Trace(const QString& message)
    {
        if (this->logLevel >= LogLevel::Trace)
        {
            std::cerr << message.toStdString() << std::endl;
        }
    }

    void Logger::Info(const QString& message)
    {
        if (this->logLevel >= LogLevel::Info)
        {
            std::cout << message.toStdString() << std::endl;
        }
    }

    void Logger::Warn(const QString& message)
    {
        if (this->logLevel >= LogLevel::Warn)
        {
            std::cerr << message.toStdString() << std::endl;
        }
    }

    void Logger::Error(const QString& message)
    {
        if (this->logLevel >= LogLevel::Error)
        {
            std::cerr << message.toStdString() << std::endl;
        }
    }

    void Logger::Fatal(const QString& message)
    {
        if (this->logLevel >= LogLevel::Fatal)
        {
            std::cerr << "FATAL:" << message.toStdString() << std::endl;
        }
    }
}
