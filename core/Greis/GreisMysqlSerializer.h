#ifndef GreisMysqlSerializer_h__
#define GreisMysqlSerializer_h__

#include <QtCore/QByteArray>
#include <QtCore/QVariant>
#include <string>
#include <vector>
#include <cassert>
#include "GreisBinarySerializer.h"
#include "GreisTypes.h"

namespace Greis
{
    class GreisMysqlSerializer
    {
    public:
        GreisMysqlSerializer()
        {
        }

        // Serialization

        // Important limitations:
        // Custom Types are not serialized this way (nor scalar or vector).
        // Only linear vectors of Greis Types are supported.

        inline QVariant SerializeChar(Types::a1 val)
        {
            return QVariant(QChar::fromLatin1(val));
        }

        inline QVariant Serialize(Types::u1 val)
        {
            return QVariant((unsigned int) val);
        }

        inline QVariant Serialize(Types::u2 val)
        {
            return QVariant((unsigned int) val);
        }

        inline QVariant Serialize(Types::u4 val)
        {
            return QVariant((unsigned int) val);
        }

        inline QVariant Serialize(Types::i1 val)
        {
            return QVariant((int) val);
        }

        inline QVariant Serialize(Types::i2 val)
        {
            return QVariant((int) val);
        }

        inline QVariant Serialize(Types::i4 val)
        {
            return QVariant((int) val);
        }

        inline QVariant Serialize(float val)
        {
            return QVariant(val);
        }

        inline QVariant Serialize(double val)
        {
            return QVariant(val);
        }

        inline QVariant Serialize(std::string val)
        {
            return QVariant(QString::fromLatin1(val.c_str(), val.size()));
        }

        // vector of Greis types
        template<typename T>
        inline QVariant Serialize(const std::vector<T>& val)
        {
            static_assert(std::is_arithmetic<T>::value, "T is not a Greis type.");

            QByteArray ba;
            _binSerializer.Serialize(val, ba);
            return ba;
        }

        inline QVariant SerializeIds(const std::vector<int>& val)
        {
            QByteArray ba;
            _binSerializer.Serialize(val, ba);
            return ba;
        }

        // Deserialization

        inline void DeserializeChar(const QVariant& val, char& out)
        {
            out = val.toString().at(0).toLatin1();
        }

        inline void Deserialize(const QVariant& val, std::string& out)
        {
            auto qs = val.toString().toLatin1();
            out = std::string(qs.constData(), qs.size());
        }

        inline void Deserialize(const QVariant& val, unsigned char& out)
        {
            out = (unsigned char) val.toUInt();
        }

        inline void Deserialize(const QVariant& val, unsigned short& out)
        {
            out = (unsigned short) val.toUInt();
        }

        inline void Deserialize(const QVariant& val, unsigned int& out)
        {
            out = val.toUInt();
        }

        inline void Deserialize(const QVariant& val, char& out)
        {
            out = (char) val.toInt();
        }

        inline void Deserialize(const QVariant& val, short& out)
        {
            out = (short) val.toInt();
        }

        inline void Deserialize(const QVariant& val, int& out)
        {
            out = val.toInt();
        }

        inline void Deserialize(const QVariant& val, float& out)
        {
            out = val.toFloat();
        }

        inline void Deserialize(const QVariant& val, double& out)
        {
            out = val.toDouble();
        }

        // vector of Greis types
        template<typename T>
        inline void Deserialize(const QVariant& val, std::vector<T>& out)
        {
            static_assert(std::is_arithmetic<T>::value, "T is not a Greis type.");

            QByteArray ba = val.toByteArray();
            _binSerializer.Deserialize(ba, ba.size(), out);
        }

        inline void DeserializeIds(const QVariant& val, std::vector<int>& out)
        {
            QByteArray ba = val.toByteArray();
            _binSerializer.Deserialize(ba, ba.size(), out);
        }
    private:
        GreisBinarySerializer _binSerializer;
    };
}

#endif // GreisMysqlSerializer_h__
