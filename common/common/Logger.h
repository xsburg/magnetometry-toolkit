#ifndef Logger_h__
#define Logger_h__

#include "Singleton.h"
#include "QtCore/QString"

namespace common
{
    enum LogLevel
    {
        Debug = 5,
        Trace = 4,
        Info = 3,
        Warn = 2,
        Error = 1,
        Fatal = 0
    };

    class Logger
    {
        SINGLETON_BLOCK(Logger)

        int logLevel;
    public:
        Logger()
        {
            logLevel = LogLevel::Debug;
        }

        void initialize(LogLevel logLevel);

        void debug(const QString& message);
        void trace(const QString& message);
        void info(const QString& message);
        void warn(const QString& message);
        void error(const QString& message);
        void fatal(const QString& message);
        void write(LogLevel logLevel, const QString& message);
    };
}

#define sLogger SINGLETON_INSTANCE(::common::Logger)

#endif // Logger_h__
