#include "NonStdTextMessage.h"
#include <boost/format.hpp>
#include "Common/SmartPtr.h"

using std::string;
using namespace Common;

namespace Greis
{
    NonStdTextMessage::NonStdTextMessage(char p_eom) : Message(EMessageKind::NonStdTextMessage)
    {
        _id = IdForEomOnlyMessage;
        _eom = p_eom;
    }

    NonStdTextMessage::NonStdTextMessage(char p_id, std::string p_body, char p_eom) 
        : Message(EMessageKind::NonStdTextMessage)
    {
        _id = p_id;
        _body = p_body;
        _eom = p_eom;
    }

    std::string NonStdTextMessage::ToString() const
    {
        string eomStr = _eom == '\r' ? "\\r" : (_eom == '\n' ? "\\n" : string(1, _eom));
        string ret = boost::str(boost::format(
            "<NonStdTextMessage id=0x%1$02X body_length=%2% eom='%3%'>%4%</NonStdTextMessage>") % (int)_id 
            % _body.size() % eomStr % _body);
        return ret;
    }

    bool NonStdTextMessage::Validate() const
    {
        if (_body.size() > 0 && (_id < MinIdValue || _id > MaxIdValue))
            return false;
        if (!IsEom(_eom))
            return false;
        return true;
    }

    NonStdTextMessage::UniquePtr_t NonStdTextMessage::CreateCarriageReturnMessage()
    {
        return make_unique<NonStdTextMessage>('\r');
    }

    NonStdTextMessage::UniquePtr_t NonStdTextMessage::CreateNewLineMessage()
    {
        return make_unique<NonStdTextMessage>('\n');
    }

    QByteArray NonStdTextMessage::ToByteArray() const
    {
        QByteArray result;
        if (_id == IdForEomOnlyMessage)
        {
            result.append(_eom);
        } else {
            result.append(_id);
            result.append(_body.c_str(), _body.size());
            result.append(_eom);
        }
        return result;
    }
}
