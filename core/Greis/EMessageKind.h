#ifndef EMessageKind_h__
#define EMessageKind_h__

namespace Greis
{
    struct EMessageKind
    {
        enum Type
        {
            UnknownMessage,
            StdMessage,
            NonStdTextMessage
        };
    };
}

#endif // EMessageKind_h__
