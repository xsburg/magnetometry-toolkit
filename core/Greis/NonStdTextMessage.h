#ifndef NonStdTextMessage_h__
#define NonStdTextMessage_h__

#include "Message.h"

namespace Greis
{
    /*
        Message identifier is any character in the range [!.../] (decimal ASCII codes in the
        range [33…47]). Message identifier is optional. If absent, the message body should have
        length zero (i.e., should be absent as well).
        Message body is a sequence of ASCII characters except <CR> (decimal code 13) and
        <LF> (decimal code 10) characters. No limitation on the body length is imposed by the
        format.
        The end of message marker is either <CR> or <LF> character.
        Note that the format allows for non-standard messages comprising only CR or LF characters.
        This feature allows to make standard GREIS message streams look more human readable
        when outputting data to a general-purpose terminal or viewing with generic
        text viewer or editor.
        One of the non-standard text message identifiers, the character “$”, is already reserved
        as the identifier for standard NMEA messages. No other non-standard text messages
        should use the “$” as identifier.
    */
    class NonStdTextMessage : public Message
    {
    public:
        SMART_PTR_T(NonStdTextMessage);

        // Zero-length non-standard Text Message
        NonStdTextMessage(char p_eom);

        // Non-standard Text Message
        NonStdTextMessage(char p_id, std::string p_body, char p_eom);

        virtual std::string ToString() const;
        virtual bool Validate() const;

        inline const char Id() const { return _id; }
        inline const std::string& Body() const { return _body; }
        virtual int Size() const { return 1 + (_id == IdForEomOnlyMessage ? 0 : _body.size() + 1); }
        virtual QByteArray ToByteArray() const;
        inline const char Eom() const { return _eom; }

        inline static bool IsEom(char ch) { return ch == '\r' || ch == '\n'; }

        static const char MinIdValue = 33;
        static const char MaxIdValue = 47;
        static NonStdTextMessage::UniquePtr_t CreateCarriageReturnMessage();
        static NonStdTextMessage::UniquePtr_t CreateNewLineMessage();
    private:
        char _id;
        std::string _body;
        char _eom;
        static const char IdForEomOnlyMessage = 0;
    };
}

#endif // NonStdTextMessage_h__
