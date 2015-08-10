#ifndef StdMessage_h__
#define StdMessage_h__

#include "Message.h"
#include "Common/InvalidOperationException.h"
#include "EMessageId.h"
#include "GreisTypes.h"
#include "GreisBinarySerializer.h"
#include "Common/NotImplementedException.h"

namespace Greis
{
    class StdMessage : public Message
    {
    public:
        SMART_PTR_T(StdMessage);

        StdMessage();

        virtual std::string ToString() const override = 0;
        virtual bool Validate() const override;
        virtual bool IsCorrect() const { return true; }
        virtual void RecalculateChecksum() = 0;

        virtual std::string Id() const = 0;
        virtual EMessageId::Type IdNumber() const = 0;
        virtual int BodySize() const = 0;
        virtual int Size() const override
        {
            return HeadSize() + BodySize();
        }
        virtual QByteArray ToByteArray() const override = 0;
        
        inline static int HeadSize() { return _headSize; }
        
        static const char MinIdValue = 48;
        static const char MaxIdValue = 126;

        static EMessageId::Type MapIdStrToEnum(char* p_id);
    protected:
        std::string toString(const std::string& tagName) const;
        static bool validateChecksum8Ascii(char* p_message, int p_length);
        static bool validateChecksum8Bin(char* p_message, int p_length);
        static std::string convertByteArrayToReadableString(char* p_message, int p_length);
        QByteArray headToByteArray() const;

        static const int _headSize = 5;
        static GreisBinarySerializer _serializer;
    };
}

#endif // StdMessage_h__
