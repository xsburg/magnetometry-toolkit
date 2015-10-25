#ifndef FileException_h__
#define FileException_h__

#include "Exception.h"

namespace common
{
	class FileException : public Exception
	{
	public:
	    FileException(const QString& message) : Exception(message)
	    {
	    }
	};
}

#endif // FileException_h__