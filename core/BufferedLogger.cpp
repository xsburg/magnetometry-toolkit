#include "BufferedLogger.h"

void core::BufferedLogger::debug(const QString& message)
{
    write(message, Common::Debug);
}

void core::BufferedLogger::trace(const QString& message)
{
    write(message, Common::Trace);
}

void core::BufferedLogger::info(const QString& message)
{
    write(message, Common::Info);
}

void core::BufferedLogger::warn(const QString& message)
{
    write(message, Common::Warn);
}

void core::BufferedLogger::error(const QString& message)
{
    write(message, Common::Error);
}

void core::BufferedLogger::fatal(const QString& message)
{
    write(message, Common::Fatal);
}

void core::BufferedLogger::write(const QString& message, Common::LogLevel logLevel)
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
