#ifndef GreisMessageStream_h__
#define GreisMessageStream_h__

#include <QtCore/QString>
#include <QtCore/QFile>
#include "Common/File.h"
#include "Common/Logger.h"
#include "Common/SmartPtr.h"
#include <string>
#include "GreisException.h"
#include "StdMessage.h"
#include "NonStdTextMessage.h"
#include "IBinaryStream.h"

namespace Greis
{
    class GreisMessageStream
    {
    public:
        SMART_PTR_T(GreisMessageStream);

        GreisMessageStream(IBinaryStream::SharedPtr_t openedBinaryStream, bool skipInvalid = false, bool handleStdOnly = true);
        bool HasNext();
        Message::UniquePtr_t Next();

    private:
        IBinaryStream::SharedPtr_t _binaryStream;
        bool _skipInvalid;
        bool _handleStdOnly;
    };
}

#endif // GreisMessageStream_h__