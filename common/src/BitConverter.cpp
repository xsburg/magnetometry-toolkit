#include "BitConverter.h"

namespace Common
{
    BitConverter::BitConverter(EByteOrder byteOrder /*= LeastSignificantByte*/)
    {
        _byteOrder = byteOrder;

        char test[4];
        *((unsigned int*)test) = 0x1;
        _machineByteOrder = test[0] == 0x1 ? LeastSignificantByte : MostSignificantByte;
    }
}
