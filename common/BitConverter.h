#pragma once

#include <stdint.h>

namespace common
{
    class BitConverter
    {
    public:
        enum EByteOrder
        {
            MostSignificantByte,
            LeastSignificantByte
        };

    private:
        EByteOrder _byteOrder;
        EByteOrder _machineByteOrder;
    public:

        explicit BitConverter(EByteOrder byteOrder = LeastSignificantByte);

        EByteOrder ByteOrder() const { return _byteOrder; }
        EByteOrder MachineByteOrder() const { return _machineByteOrder; }

        void ToByteArray(uint64_t val, char* dst) const
        {
            *reinterpret_cast<uint64_t*>(dst) = val;
            if (_machineByteOrder != _byteOrder)
            {
                endianSwap(*reinterpret_cast<uint64_t*>(dst));
            }
        }

        void ToByteArray(uint32_t val, char* dst) const
        {
            *reinterpret_cast<uint32_t*>(dst) = val;
            if (_machineByteOrder != _byteOrder)
            {
                endianSwap(*reinterpret_cast<uint32_t*>(dst));
            }
        }

        void ToByteArray(uint16_t val, char* dst) const
        {
            *reinterpret_cast<uint16_t*>(dst) = val;
            if (_machineByteOrder != _byteOrder)
            {
                endianSwap(*reinterpret_cast<uint16_t*>(dst));
            }
        }

        void ToByteArray(uint8_t val, char* dst) const
        {
            *reinterpret_cast<uint8_t*>(dst) = val;
        }

        void ToByteArray(int64_t val, char* dst) const
        {
            *reinterpret_cast<int64_t*>(dst) = val;
            if (_machineByteOrder != _byteOrder)
            {
                endianSwap(*reinterpret_cast<uint64_t*>(dst));
            }
        }

        void ToByteArray(int32_t val, char* dst) const
        {
            *reinterpret_cast<int32_t*>(dst) = val;
            if (_machineByteOrder != _byteOrder)
            {
                endianSwap(*reinterpret_cast<uint32_t*>(dst));
            }
        }

        void ToByteArray(int16_t val, char* dst) const
        {
            *reinterpret_cast<int16_t*>(dst) = val;
            if (_machineByteOrder != _byteOrder)
            {
                endianSwap(*reinterpret_cast<uint16_t*>(dst));
            }
        }

        void ToByteArray(int8_t val, char* dst) const
        {
            *reinterpret_cast<int8_t*>(dst) = val;
        }

        void ToByteArray(char val, char* dst) const
        {
            *dst = val;
        }

        void ToByteArray(float val, char* dst) const
        {
            *reinterpret_cast<float*>(dst) = val;
            if (_machineByteOrder != _byteOrder)
            {
                endianSwap(*reinterpret_cast<uint32_t*>(dst));
            }
        }

        void ToByteArray(double val, char* dst) const
        {
            *reinterpret_cast<double*>(dst) = val;
            if (_machineByteOrder != _byteOrder)
            {
                endianSwap(*reinterpret_cast<uint64_t*>(dst));
            }
        }

        uint8_t GetUInt8(const char* data) const
        {
            const uint8_t* ucdata = reinterpret_cast<const uint8_t*>(data);
            return *ucdata;
        }

        uint16_t GetUInt16(const char* data) const
        {
            const uint8_t* ucdata = reinterpret_cast<const uint8_t*>(data);
            uint16_t val = static_cast<uint16_t>((static_cast<uint16_t>(*ucdata) << 8) + *(ucdata+1));
            if (_byteOrder == LeastSignificantByte)
            {
                endianSwap(val);
            }
            return val;
        }

        uint32_t GetUInt32(const char* data) const
        {
            const uint8_t* ucdata = reinterpret_cast<const uint8_t*>(data);
            uint32_t val = static_cast<uint32_t>(
                (static_cast<uint32_t>(*ucdata) << 24) + 
                (static_cast<uint32_t>(*(ucdata+1)) << 16) + 
                (static_cast<uint32_t>(*(ucdata+2)) << 8) + 
                *(ucdata+3));
            if (_byteOrder == LeastSignificantByte)
            {
                endianSwap(val);
            }
            return val;
        }

        uint64_t GetUInt64(const char* data) const
        {
            const uint8_t* ucdata = reinterpret_cast<const uint8_t*>(data);
            uint64_t val = static_cast<uint64_t>(
                (static_cast<uint64_t>(*ucdata) << 56) + 
                (static_cast<uint64_t>(*(ucdata+1)) << 48) + 
                (static_cast<uint64_t>(*(ucdata+2)) << 40) + 
                (static_cast<uint64_t>(*(ucdata+3)) << 32) + 
                (static_cast<uint64_t>(*(ucdata+4)) << 24) + 
                (static_cast<uint64_t>(*(ucdata+5)) << 16) + 
                (static_cast<uint64_t>(*(ucdata+6)) << 8) + 
                *(ucdata+7));
            if (_byteOrder == LeastSignificantByte)
            {
                endianSwap(val);
            }
            return val;
        }

        char GetChar(const char* data) const
        {
            return *data;
        }

        int8_t GetInt8(const char* data) const
        {
            return *reinterpret_cast<const int8_t*>(data);
        }

        int16_t GetInt16(const char* data) const
        {
            uint16_t uval = GetUInt16(data);
            int16_t val = *reinterpret_cast<int16_t*>(&uval);
            return val;
        }

        int32_t GetInt32(const char* data) const
        {
            uint32_t uval = GetUInt32(data);
            int32_t val = *reinterpret_cast<int32_t*>(&uval);
            return val;
        }

        int64_t GetInt64(const char* data) const
        {
            uint64_t uval = GetUInt64(data);
            int64_t val = *reinterpret_cast<int64_t*>(&uval);
            return val;
        }

        float GetFloat(const char* data) const
        {
            uint32_t uiVal = GetUInt32(data);
            float fVal = *reinterpret_cast<float*>(&uiVal);
            return fVal;
        }

        double GetDouble(const char* data) const
        {
            uint64_t uval = GetUInt64(data);
            double dVal = *reinterpret_cast<double*>(&uval);
            return dVal;
        }

    private:
        static void endianSwap(uint16_t& x)
        {
            x = static_cast<uint16_t>(x>>8 | x<<8);
        }

        static void endianSwap(uint32_t& x)
        {
            x = x>>24 | 
                x<<8 & 0x00FF0000 |
                x>>8 & 0x0000FF00 |
                x<<24;
        }

        static void endianSwap(uint64_t& x)
        {
            x = x>>56 | 
                x<<40 & 0x00FF000000000000 |
                x<<24 & 0x0000FF0000000000 |
                x<<8  & 0x000000FF00000000 |
                x>>8  & 0x00000000FF000000 |
                x>>24 & 0x0000000000FF0000 |
                x>>40 & 0x000000000000FF00 |
                x<<56;
        }
    };
}
