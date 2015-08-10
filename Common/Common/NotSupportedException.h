#ifndef NotSupportedException_h__
#define NotSupportedException_h__

#include "Exception.h"

namespace Common
{
    class NotSupportedException : public Exception
    {
    public:
        NotSupportedException(const QString& message) : Exception(message)
        {
        }

        NotSupportedException()
        {
        }
    };
}

#endif // NotSupportedException_h__