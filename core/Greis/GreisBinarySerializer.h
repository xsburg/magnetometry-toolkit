#ifndef GreisBinarySerializer_h__
#define GreisBinarySerializer_h__

#include <QtCore/QByteArray>
#include <string>
#include <vector>
#include <cassert>
#include "Common/BitConverter.h"
#include "Common/SmartPtr.h"

namespace Greis
{
    class GreisBinarySerializer
    {
    public:
        GreisBinarySerializer(Common::BitConverter::EByteOrder byteOrder = 
            Common::BitConverter::LeastSignificantByte);

        // Serialization

        template<typename T>
        void Serialize(T val, QByteArray& outStream)
        {
            static_assert(std::is_arithmetic<T>::value && sizeof(T) > 1, "T is not a Greis type.");

            char buffer[sizeof(T)];
            _bitConverter.ToByteArray(val, buffer);
            outStream.append(buffer, sizeof(T));
        }

        template<typename T>
        void Serialize(const std::vector<T>& val, QByteArray& outStream)
        {
            for (auto it = val.begin(); it != val.end(); ++it)
            {
                Serialize(*it, outStream);
            }
        }

        template<typename T>
        void Serialize(const std::unique_ptr<T>& val, QByteArray& outStream)
        {
            outStream.append(val->ToByteArray());
        }

        void Serialize(char val, QByteArray& outStream)
        {
            outStream.append(&val, 1);
        }

        void Serialize(unsigned char val, QByteArray& outStream)
        {
            outStream.append((char*)&val, 1);
        }

        void Serialize(const std::string& val, QByteArray& outStream)
        {
            outStream.append(val.c_str(), val.size());
        }

        // Deserialization

        void Deserialize(const char* data, unsigned char& retVal)
        {
            retVal = *((unsigned char*)data);
        }

        void Deserialize(const char* data, unsigned short& retVal)
        {
            retVal = _bitConverter.GetUShort(data);
        }

        void Deserialize(const char* data, unsigned int& retVal)
        {
            retVal = _bitConverter.GetUInt(data);
        }

        void Deserialize(const char* data, char& retVal)
        {
            retVal = *data;
        }

        void Deserialize(const char* data, short& retVal)
        {
            retVal = _bitConverter.GetShort(data);
        }

        void Deserialize(const char* data, int& retVal)
        {
            retVal = _bitConverter.GetInt(data);
        }

        void Deserialize(const char* data, float& retVal)
        {
            retVal = _bitConverter.GetFloat(data);
        }

        void Deserialize(const char* data, double& retVal)
        {
            retVal = _bitConverter.GetDouble(data);
        }

        void Deserialize(const char* data, int length, std::string& retVal)
        {
            retVal = std::string(data, length);
        }

        template<typename T>
        void Deserialize(const char* data, int length, std::unique_ptr<T>& retVal)
        {
            retVal = Common::make_unique<T>(data, length);
        }

        // Deserialization for std::vector<Greis::Type>
        template<typename T>
        void Deserialize(const char* data, int length, std::vector<T>& retVal)
        {
            static_assert(std::is_arithmetic<T>::value, "T is not a Greis type.");
            int itemSize = sizeof(T);
            assert(length % itemSize == 0);
            retVal.clear();
            for (int i = 0; i < length; i += itemSize)
            {
                T val;
                Deserialize(data + i, val);
                retVal.push_back(std::move(val));
            }
        }

        // Deserialization for std::vector<std::string> is not possible

        // Deserialization for std::vector<CustomType::UniquePtr_t> && std::vector<std::vector<...>> 
        // (where ... is GreisType or vector)
        template<typename T>
        void Deserialize(const char* data, int length, int itemSize, std::vector<T>& retVal)
        {
            assert(length % itemSize == 0);
            retVal.clear();
            for (int i = 0; i < length; i += itemSize)
            {
                T val;
                Deserialize(data + i, itemSize, val);
                retVal.push_back(std::move(val));
            }
        }
    private:
        Common::BitConverter _bitConverter;
    };
}

#endif // GreisBinarySerializer_h__
