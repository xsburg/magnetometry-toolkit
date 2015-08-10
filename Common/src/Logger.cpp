#include "Logger.h"

#include <QtCore/QtCore>
#include <iostream>

namespace Common
{
    void Logger::Initialize(int logLevel)
    {
        if (logLevel < 0)
        {
            logLevel = 5;
        }

        this->logLevel = logLevel;
    }

    void Logger::Debug(const QString& message)
    {
        if (this->logLevel >= 5)
        {
            std::cerr << message.toStdString() << std::endl;
        }
    }

    void Logger::Trace(const QString& message)
    {
        if (this->logLevel >= 4)
        {
            std::cerr << message.toStdString() << std::endl;
        }
    }

    void Logger::Info(const QString& message)
    {
        if (this->logLevel >= 3)
        {
            std::cout << message.toStdString() << std::endl;
        }
    }

    void Logger::Warn(const QString& message)
    {
        if (this->logLevel >= 2)
        {
            std::cerr << message.toStdString() << std::endl;
        }
    }

    void Logger::Error(const QString& message)
    {
        if (this->logLevel >= 1)
        {
            std::cerr << message.toStdString() << std::endl;
        }
    }

    void Logger::Fatal(const QString& message)
    {
        if (this->logLevel >= 0)
        {
            std::cerr << "FATAL:" << message.toStdString() << std::endl;
        }
    }
}
