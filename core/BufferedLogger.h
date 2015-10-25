// ***********************************************************************
// <author>Stepan Burguchev</author>
// <copyright company="Comindware">
//   Copyright (c) Comindware 2010-2015. All rights reserved.
// </copyright>
// <summary>
//   BufferedLogger.h
// </summary>
// ***********************************************************************
#pragma once

#include <QtCore/QtCore>
#include "common/Logger.h"

namespace core
{
    struct BufferMessage
    {
        QDateTime time;
        QString message;
        QString logLevel;
        uint64_t id;
    };

    class BufferedLogger
    {
        int maxBufferSize;
        int id;
        QList<BufferMessage> messageBuffer;
    public:
        SMART_PTR_T(BufferedLogger);

        explicit BufferedLogger(int maxBufferSize = 1000)
        {
            this->id = 1;
            this->maxBufferSize = maxBufferSize;
        }

        void debug(const QString& message);
        void trace(const QString& message);
        void info(const QString& message);
        void warn(const QString& message);
        void error(const QString& message);
        void fatal(const QString& message);
        void write(common::LogLevel logLevel, const QString& message);

        const QList<BufferMessage>& buffer() const
        {
            return messageBuffer;
        }
    };
}
