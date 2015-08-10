#ifndef Message_h__
#define Message_h__

#include <boost/utility.hpp>
#include <QtCore/QByteArray>
#include "Common/SmartPtr.h"
#include "EMessageKind.h"

namespace Greis
{
    class Message : private boost::noncopyable
    {
    private:
        const EMessageKind::Type _kind;
    public:
        SMART_PTR_T(Message);

        explicit Message(EMessageKind::Type p_kind) : _kind(p_kind)
        {
        }

        virtual ~Message()
        {
        }

        virtual std::string ToString() const = 0;
        virtual bool Validate() const = 0;
        virtual int Size() const = 0;
        virtual QByteArray ToByteArray() const = 0;

        inline EMessageKind::Type Kind() const { return _kind; }
    };
}

#endif // Message_h__
