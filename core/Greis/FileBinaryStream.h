#pragma once

#include "IBinaryStream.h"
#include "Common/File.h"
#include <Common/NotSupportedException.h>

namespace Greis
{
    enum FileBinaryStreamOpenMode
    {
        Read,
        Create
    };

    class FileBinaryStream : public IBinaryStream
    {
    private:
        Common::QFilePtr _file;
    public:
        SMART_PTR_T(FileBinaryStream);

        FileBinaryStream(QString filename, FileBinaryStreamOpenMode openMode = Read)
        {
            if (openMode == Read)
            {
                _file = Common::File::OpenReadBinary(filename);
            } else if (openMode == Create)
            {
                _file = Common::File::CreateBinary(filename);
            }
        }

        int pos() const { return _file->pos(); }

        void write(QByteArray data)
        {
            _file->write(data);
        }

        void flush()
        {
            _file->flush();
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

        void close()
        {
            return _file->close();
        }

        void purgeBuffers()
        {
            throw Common::NotSupportedException("Buffers purge is not supported in FileBinaryStream.");
        }
    };
}
