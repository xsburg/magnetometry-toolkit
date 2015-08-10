#pragma once

#include "IBinaryStream.h"

namespace Greis
{
    class LoggingBinaryStream : public IBinaryStream
    {
    private:
        IBinaryStream::SharedPtr_t _loggedStream;

        IBinaryStream::SharedPtr_t _logStorage;
    public:
        SMART_PTR_T(LoggingBinaryStream);

        LoggingBinaryStream(IBinaryStream::SharedPtr_t loggedStream, IBinaryStream::SharedPtr_t logStorage)
        {
            _loggedStream = loggedStream;
            _logStorage = logStorage;
        }

        virtual ~LoggingBinaryStream()
        {
            _logStorage->close();
        }

        void write(QByteArray data)
        {
            _loggedStream->write(data);
        }

        QByteArray read(qint64 maxlen)
        {
            QByteArray data = _loggedStream->read(maxlen);
            _logStorage->write(data);
            _logStorage->flush();
            return data;
        }

        QByteArray peek(qint64 maxlen)
        {
            return _loggedStream->peek(maxlen);
        }

        qint64 read(char * data, qint64 maxSize)
        {
            qint64 readCount = _loggedStream->read(data, maxSize);
            _logStorage->write(QByteArray(data, (int)readCount));
            _logStorage->flush();
            return readCount;
        }

        qint64 peek(char * data, qint64 maxSize)
        {
            return _loggedStream->peek(data, maxSize);
        }

        void flush()
        {
            _loggedStream->flush();
            _logStorage->flush();
        }

        void close()
        {
            _loggedStream->close();
        }

        void purgeBuffers()
        {
            _loggedStream->purgeBuffers();
        }
    };
}
