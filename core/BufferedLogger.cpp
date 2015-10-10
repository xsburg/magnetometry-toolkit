#include "BufferedLogger.h"

void core::BufferedLogger::debug(const QString& message)
{
    write(Common::Debug, message);
}

void core::BufferedLogger::trace(const QString& message)
{
    write(Common::Trace, message);
}

void core::BufferedLogger::info(const QString& message)
{
    write(Common::Info, message);
}

void core::BufferedLogger::warn(const QString& message)
{
    write(Common::Warn, message);
}

void core::BufferedLogger::error(const QString& message)
{
    write(Common::Error, message);
}

void core::BufferedLogger::fatal(const QString& message)
{
    write(Common::Fatal, message);
}

void core::BufferedLogger::write(Common::LogLevel logLevel, const QString& message)
{
    QString levelMsg;
    switch (logLevel)
    {
    case Common::Debug:
        levelMsg = "DEBUG";
        break;
    case Common::Trace:
        levelMsg = "TRACE";
        break;
    case Common::Info:
        levelMsg = "INFO";
        break;
    case Common::Warn:
        levelMsg = "WARN";
        break;
    case Common::Error:
        levelMsg = "ERROR";
        break;
    case Common::Fatal:
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
