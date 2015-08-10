#ifndef StdMessageFactory_h__
#define StdMessageFactory_h__

#include "StdMessage.h"

namespace Greis
{
    class StdMessageFactory
    {
    public:
        static StdMessage::UniquePtr_t Create(char* p_message, int p_length);
    private:
        static StdMessage::UniquePtr_t createById(EMessageId::Type id, char* p_message, int p_length);
    };
}

#endif // StdMessageFactory_h__
