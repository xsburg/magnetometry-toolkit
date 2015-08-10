#pragma once

#include <QtCore/QtCore>

#include "Common/SmartPtr.h"

namespace Greis
{
    class IBinaryStream
    {
    public:
        virtual ~IBinaryStream()
        {
        }

        SMART_PTR_T(IBinaryStream);

        virtual void write(QByteArray data) = 0;
        virtual QByteArray read(qint64 maxlen) = 0;
        virtual qint64 read(char * data, qint64 maxSize) = 0;
        virtual QByteArray peek(qint64 maxlen) = 0;
        virtual qint64 peek(char * data, qint64 maxSize) = 0;
        virtual void close() = 0;
        virtual void purgeBuffers() = 0;
        virtual void flush() = 0;
    };
}
