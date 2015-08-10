#ifndef RawCustomType_h__
#define RawCustomType_h__

#include "CustomType.h"

namespace Greis
{
    class RawCustomType : public CustomType
    {
    public:
        SMART_PTR_T(RawCustomType);

        RawCustomType(const char* p_data, int p_length) : _message(p_data, p_length)
        {
        }

        virtual ECustomTypeId::Type IdNumber() const { return ECustomTypeId::Unknown; }
        virtual QByteArray ToByteArray() const { return _message; };
        virtual int Size() const { return _message.size(); };

    private:
        QByteArray _message; // Id+Len+Body
    };
}

#endif // RawCustomType_h__
