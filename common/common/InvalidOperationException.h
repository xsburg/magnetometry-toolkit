#ifndef InvalidOperationException_h__
#define InvalidOperationException_h__

#include "Exception.h"

namespace Common
{
    class InvalidOperationException : public Exception
    {
    public:
        InvalidOperationException(const QString& message) : Exception(message)
        {
        }

        InvalidOperationException()
        {
        }
    };
}

#endif // InvalidOperationException_h__