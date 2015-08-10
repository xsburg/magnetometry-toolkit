#include "ChecksumComputer.h"

namespace Greis
{
    Types::u1 ChecksumComputer::ComputeCs8(void const* source, int count, Types::u1 initVal)
    {
        Types::u1 const* src = (Types::u1 const*)source;
        // empty array
        if (count == 0)
        {
            return initVal;
        }
        // initialize
        Types::u1 res = 0;
        if (initVal != 0)
        {
            // ROT_LEFT was on previous step (in return statement)
            res = initVal ^ *src++;
            --count;
        }
        // act
        while(count--)
            res = ROT_LEFT(res) ^ *src++;
        return ROT_LEFT(res);
    }

    enum
    {
        WIDTH = 16,                     // Width of poly
        POLY = 0x1021,                  // Poly. Bit #16 is set and hidden
        BYTE_BITS = 8,                  // Number of bits in byte
        TABLE_SIZE = 1 << BYTE_BITS,    // Size of table
        MSB_MASK = 1 << (WIDTH - 1)     // Mask for high order bit in a word
    };

    ChecksumComputer::Crc16* ChecksumComputer::table()
    {
        static Crc16 aTable[TABLE_SIZE];
        static bool firstCall = true;
        if (firstCall)
        {
            crc16init(aTable);
            firstCall = false;
        }
        return aTable;
    }

    void ChecksumComputer::crc16init(Crc16* table)
    {
        Crc16 i;
        for(i = 0; i < TABLE_SIZE; ++i)
        {
            Crc16 val = i << (WIDTH - BYTE_BITS);
            int j;
            for(j = 0; j < BYTE_BITS; ++j)
                val = (val << 1) ^ ((val & MSB_MASK) ? POLY : 0);
            table[i] = val;
        }
    }

    ChecksumComputer::Crc16 ChecksumComputer::ComputeCrc16(void const* src, int cnt, Crc16 crc)
    {
        unsigned char const* s = (unsigned char const*)src;
        while(cnt--)
        {
            crc = (crc << BYTE_BITS) ^ table()[(crc >> (WIDTH - BYTE_BITS)) ^ *s++];
        }
        return crc;
    }
}
