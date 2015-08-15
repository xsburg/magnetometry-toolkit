#ifndef BitConverter_h__
#define BitConverter_h__

namespace Common
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

        BitConverter(EByteOrder byteOrder = LeastSignificantByte);

        EByteOrder ByteOrder() const { return _byteOrder; }
        EByteOrder MachineByteOrder() const { return _machineByteOrder; }

        inline void ToByteArray(unsigned long long val, char* dst) const
        {
            *((unsigned long long*)dst) = val;
            if (_machineByteOrder != _byteOrder)
            {
                endianSwap(*((unsigned long long*)dst));
            }
        }

        inline void ToByteArray(unsigned int val, char* dst) const
        {
            *((unsigned int*)dst) = val;
            if (_machineByteOrder != _byteOrder)
            {
                endianSwap(*((unsigned int*)dst));
            }
        }

        inline void ToByteArray(unsigned short val, char* dst) const
        {
            *((unsigned short*)dst) = val;
            if (_machineByteOrder != _byteOrder)
            {
                endianSwap(*((unsigned short*)dst));
            }
        }

        inline void ToByteArray(unsigned char val, char* dst) const
        {
            *((unsigned char*)dst) = val;
        }

        inline void ToByteArray(long long val, char* dst) const
        {
            *((long long*)dst) = val;
            if (_machineByteOrder != _byteOrder)
            {
                endianSwap(*((unsigned long long*)dst));
            }
        }

        inline void ToByteArray(int val, char* dst) const
        {
            *((int*)dst) = val;
            if (_machineByteOrder != _byteOrder)
            {
                endianSwap(*((unsigned int*)dst));
            }
        }

        inline void ToByteArray(short val, char* dst) const
        {
            *((short*)dst) = val;
            if (_machineByteOrder != _byteOrder)
            {
                endianSwap(*((unsigned short*)dst));
            }
        }

        inline void ToByteArray(char val, char* dst) const
        {
            *dst = val;
        }

        inline void ToByteArray(float val, char* dst) const
        {
            *((float*)dst) = val;
            if (_machineByteOrder != _byteOrder)
            {
                endianSwap(*((unsigned int*)dst));
            }
        }

        inline void ToByteArray(double val, char* dst) const
        {
            *((double*)dst) = val;
            if (_machineByteOrder != _byteOrder)
            {
                endianSwap(*((unsigned long long*)dst));
            }
        }

        inline unsigned char GetUChar(const char* data) const
        {
            unsigned const char* ucdata = reinterpret_cast<unsigned const char*>(data);
            return *ucdata;
        }

        inline unsigned short GetUShort(const char* data) const
        {
            unsigned const char* ucdata = reinterpret_cast<unsigned const char*>(data);
            unsigned short val = (unsigned short)((((unsigned short)*ucdata) << 8) + (*(ucdata+1)));
            if (_byteOrder == LeastSignificantByte)
            {
                endianSwap(val);
            }
            return val;
        }

        inline unsigned int GetUInt(const char* data) const
        {
            unsigned const char* ucdata = reinterpret_cast<unsigned const char*>(data);
            unsigned int val = (unsigned int)(
                                (((unsigned int)*ucdata) << 24) + 
                                (((unsigned int)*(ucdata+1)) << 16) + 
                                (((unsigned int)*(ucdata+2)) << 8) + 
                                (*(ucdata+3)));
            if (_byteOrder == LeastSignificantByte)
            {
                endianSwap(val);
            }
            return val;
        }

        inline unsigned long long GetULongLong(const char* data) const
        {
            unsigned const char* ucdata = reinterpret_cast<unsigned const char*>(data);
            unsigned long long val = (unsigned long long)(
                (((unsigned long long)*ucdata) << 56) + 
                (((unsigned long long)*(ucdata+1)) << 48) + 
                (((unsigned long long)*(ucdata+2)) << 40) + 
                (((unsigned long long)*(ucdata+3)) << 32) + 
                (((unsigned long long)*(ucdata+4)) << 24) + 
                (((unsigned long long)*(ucdata+5)) << 16) + 
                (((unsigned long long)*(ucdata+6)) << 8) + 
                (*(ucdata+7)));
            if (_byteOrder == LeastSignificantByte)
            {
                endianSwap(val);
            }
            return val;
        }

        inline char GetChar(const char* data) const
        {
            return *data;
        }

        inline short GetShort(const char* data) const
        {
            unsigned short uval = GetUShort(data);
            int val = *(reinterpret_cast<short*>(&uval));
            return val;
        }

        inline int GetInt(const char* data) const
        {
            unsigned int uval = GetUInt(data);
            int val = *(reinterpret_cast<int*>(&uval));
            return val;
        }

        inline long long GetLongLong(const char* data) const
        {
            unsigned long long uval = GetULongLong(data);
            long long val = *(reinterpret_cast<long long*>(&uval));
            return val;
        }

        inline float GetFloat(const char* data) const
        {
            unsigned int uiVal = GetUInt(data);
            float fVal = *(reinterpret_cast<float*>(&uiVal));
            return fVal;
        }

        inline double GetDouble(const char* data) const
        {
            unsigned long long uval = GetULongLong(data);
            double dVal = *(reinterpret_cast<double*>(&uval));
            return dVal;
        }

    private:
        inline static void endianSwap(unsigned short& x)
        {
            x = (unsigned short)
                ((x>>8) | 
                (x<<8));
        }

        inline static void endianSwap(unsigned int& x)
        {
            x = (x>>24) | 
                ((x<<8) & 0x00FF0000) |
                ((x>>8) & 0x0000FF00) |
                (x<<24);
        }

        // __int64 for MSVC, "long long" for gcc
        inline static void endianSwap(unsigned long long& x)
        {
            x = (x>>56) | 
                ((x<<40) & 0x00FF000000000000) |
                ((x<<24) & 0x0000FF0000000000) |
                ((x<<8)  & 0x000000FF00000000) |
                ((x>>8)  & 0x00000000FF000000) |
                ((x>>24) & 0x0000000000FF0000) |
                ((x>>40) & 0x000000000000FF00) |
                (x<<56);
        }
    };
}

#endif // BitConverter_h__
