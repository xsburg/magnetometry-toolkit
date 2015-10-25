#include "BufferedLogger.h"

void core::BufferedLogger::debug(const QString& message)
{
    write(common::Debug, message);
}

void core::BufferedLogger::trace(const QString& message)
{
    write(common::Trace, message);
}

void core::BufferedLogger::info(const QString& message)
{
    write(common::Info, message);
}

void core::BufferedLogger::warn(const QString& message)
{
    write(common::Warn, message);
}

void core::BufferedLogger::error(const QString& message)
{
    write(common::Error, message);
}

void core::BufferedLogger::fatal(const QString& message)
{
    write(common::Fatal, message);
}

void core::BufferedLogger::write(common::LogLevel logLevel, const QString& message)
{
    QString levelMsg;
    switch (logLevel)
    {
    case common::Debug:
        levelMsg = "DEBUG";
        break;
    case common::Trace:
        levelMsg = "TRACE";
        break;
    case common::Info:
        levelMsg = "INFO";
        break;
    case common::Warn:
        levelMsg = "WARN";
        break;
    case common::Error:
        levelMsg = "ERROR";
        break;
    case common::Fatal:
        levelMsg = "FATAL";
        break;
    default:
        break;
    }

    BufferMessage result;
    result.time = QDateTime::currentDateTimeUtc();
    result.logLevel = levelMsg;
    result.message = message;
    result.id = this->id++;

    //auto result = QString("%1 [%2]: %3").arg(QDateTime::currentDateTimeUtc().toString(Qt::DateFormat::ISODate), levelMsg, message);
    messageBuffer.push_back(result);
    if (messageBuffer.size() > this->maxBufferSize)
    {
        messageBuffer.pop_front();
    }
}
