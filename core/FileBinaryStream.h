#pragma once

#include "IBinaryStream.h"
#include "common/File.h"

namespace core
{
    class FileBinaryStream : public IBinaryStream
    {
    private:
        common::QFilePtr _file;
    public:
        SMART_PTR_T(FileBinaryStream);

        FileBinaryStream(QString filename, bool create)
        {
            if (create)
            {
                _file = common::File::CreateBinary(filename);
            }
            else
            {
                _file = common::File::OpenReadBinary(filename);
            }
        }

        ~FileBinaryStream()
        {
            _file->close();
        }

        int pos() const { return _file->pos(); }

        void close()
        {
            _file->close();
        }

        void write(QByteArray data)
        {
            _file->write(data);
        }

        void write(const char* data, qint64 len)
        {
            _file->write(data, len);
        }

        QByteArray read(qint64 maxlen)
        {
            return _file->read(maxlen);
        }

        QByteArray peek(qint64 maxlen)
        {
            return _file->peek(maxlen);
        }

        qint64 read(char * data, qint64 maxSize)
        {
            return _file->read(data, maxSize);
        }

        qint64 peek(char * data, qint64 maxSize)
        {
            return _file->peek(data, maxSize);
        }

        bool flush() override
        {
            return _file->flush();
        }
    };
}
