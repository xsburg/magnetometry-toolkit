#pragma once

#include <QtCore/QtCore>

#include "Common/SmartPtr.h"

namespace core
{
    class IBinaryStream
    {
    public:
        SMART_PTR_T(IBinaryStream);

        virtual void close() = 0;
        virtual void write(QByteArray data) = 0;
        virtual void write(const char* data, qint64 len) = 0;
        virtual QByteArray read(qint64 maxlen) = 0;
        virtual qint64 read(char * data, qint64 maxSize) = 0;
        virtual QByteArray peek(qint64 maxlen) = 0;
        virtual qint64 peek(char * data, qint64 maxSize) = 0;
    };
}
