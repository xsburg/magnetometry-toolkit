#include "RawStdMessage.h"
#include "GreisTypes.h"
#include "ChecksumComputer.h"
#include "boost/format.hpp"

using std::string;
using std::vector;
using namespace Common;

namespace Greis
{
    RawStdMessage::RawStdMessage(const char* p_message, int p_length) : _message(p_message, p_length)
    {
    }

    std::string RawStdMessage::ToString() const
    {
        return toString("RawStdMessage");
    }

    void RawStdMessage::RecalculateChecksum()
    {
    }
}
