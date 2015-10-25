#ifndef DatabaseException_h__
#define DatabaseException_h__

#include "Exception.h"

namespace common 
{
    class DatabaseException : public Exception
    {
    public:
        DatabaseException(const QString& message) : Exception(message)
        {
        }
    };
}

#endif // DatabaseException_h__