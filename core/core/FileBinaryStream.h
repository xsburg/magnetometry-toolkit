#pragma once

#include "IBinaryStream.h"
#include "Common/File.h"

namespace Greis
{
    class FileBinaryStream : public IBinaryStream
    {
    private:
        Common::QFilePtr _file;
    public:
        SMART_PTR_T(FileBinaryStream);

        FileBinaryStream(QString filename)
        {
            _file = Common::File::OpenReadBinary(filename);
        }

        int pos() const { return _file->pos(); }

        void write(QByteArray data)
        {
            _file->write(data);
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
    };
}
