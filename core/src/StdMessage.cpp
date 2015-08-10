#include "StdMessage.h"
#include "GreisTypes.h"
#include "ChecksumComputer.h"
#include "boost/format.hpp"
#include "Common/BitConverter.h"
#include "RawStdMessage.h"
#include <iostream>

using std::string;
using std::vector;
using namespace Common;

namespace Greis
{
    StdMessage::StdMessage() : Message(EMessageKind::StdMessage)
    {
    }

    bool StdMessage::Validate() const
    {
        string aId = Id();
        if (aId[0] < MinIdValue || aId[0] > MaxIdValue || 
            aId[1] < MinIdValue || aId[1] > MaxIdValue)
        {
            return false;
        }

        return true;
    }

    bool StdMessage::validateChecksum8Ascii(char* p_message, int p_length)
    {
        // expected
        if (p_length < HeadSize() + 2)
        {
            throw Exception("Body too small.");
        }
        if (p_length == HeadSize() + 2)
        {
            return true;
        }
        bool bOk;
        Types::u1 expected = QString::fromLatin1(p_message + p_length - 2, 2).toUInt(&bOk, 16);
        if (!bOk)
        {
            return false;
        }
        // actual
        Types::u1 actual = ChecksumComputer::ComputeCs8(p_message, p_length - 2);
        // check
        return expected == actual;
    }

    bool StdMessage::validateChecksum8Bin(char* p_message, int p_length)
    {
        // expected
        if (p_length < HeadSize() + 1)
        {
            throw Exception("Body too small.");
        }
        if (p_length == HeadSize() + 1)
        {
            return true;
        }
        Types::u1 expected = ((Types::u1*)p_message)[p_length - 1];
        // actual
        Types::u1 actual = ChecksumComputer::ComputeCs8(p_message, p_length - 1);
        // check
        return expected == actual;
    }

    std::string StdMessage::convertByteArrayToReadableString( char* p_message, int p_length )
    {
        string ret;
        for (int i = 0; i < p_length; ++i)
        {
            char ch = p_message[i];
            if (isprint((unsigned char)ch))
            {
                ret.push_back(ch);
            }
            else
            {
                ret.append(boost::str(boost::format("{0x%1$02X}") % (unsigned int)(unsigned char)ch));
            }
        }
        return ret;
    }

    std::string StdMessage::toString( const std::string& tagName ) const
    {
        auto messageBytes = ToByteArray();
        auto bodyStr = convertByteArrayToReadableString(messageBytes.data() + 5, messageBytes.size() - 5);
        string ret = boost::str(boost::format("<%1% id='%2%' body_length=%3%>%4%</%1%>") 
            % tagName % Id() % BodySize() % bodyStr);
        return ret;
    }

    QByteArray StdMessage::headToByteArray() const
    {
        QByteArray head;
        head.append(Id().c_str(), 2);
        auto bodySizeStr = QString::number(BodySize(), 16).toUpper().rightJustified(3, '0', true);
        head.append(bodySizeStr.toLatin1(), 3);
        return head;
    }

    Greis::GreisBinarySerializer StdMessage::_serializer;
}
