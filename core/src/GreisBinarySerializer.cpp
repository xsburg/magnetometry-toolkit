#include "GreisBinarySerializer.h"

namespace Greis
{
    GreisBinarySerializer::GreisBinarySerializer( Common::BitConverter::EByteOrder byteOrder ) 
        : _bitConverter(byteOrder)
    {
    }
}