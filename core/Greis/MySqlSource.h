#ifndef MySqlSource_h__
#define MySqlSource_h__

#include <functional>
#include <boost/noncopyable.hpp>
#include <QtCore/QMap>
#include <QtCore/QDateTime>
#include "Common/Connection.h"
#include "Common/SmartPtr.h"
#include "Common/NotSupportedException.h"
#include "Common/NotImplementedException.h"
#include "Common/DataConsistencyException.h"
#include "EMessageId.h"
#include "ECustomTypeId.h"
#include "DataChunk.h"
#include "GreisMysqlSerializer.h"
#include "CustomType.h"

using namespace Common;

namespace Greis
{
    inline unsigned short __CHAR2_TO_USHORT(const char* c_id)
    {
        return (((unsigned short)(c_id)[0]) << 8) | (c_id)[1];
    }

    class MySqlSource : private boost::noncopyable
    {
    private:
        struct MessageEx
        {
            MessageEx(Message::UniquePtr_t msg, int epochIndex) : msg(std::move(msg)), epochIndex(epochIndex)
            {
            }

            Message::UniquePtr_t msg;
            int epochIndex;
        };
    public:
        SMART_PTR_T(MySqlSource);

        MySqlSource(Connection* connection);
        ~MySqlSource();

        DataChunk::UniquePtr_t ReadRange(const QDateTime& from, const QDateTime& to);
        DataChunk::UniquePtr_t ReadAll();
    private:
        DataChunk::UniquePtr_t read(const QString& sqlWhere = "");

        void handleMessage(QString queryStr, std::function<Message*(const std::basic_string<char>&, int, const QSqlQuery&)> handleMessageFields, std::map<unsigned long long, std::vector<MessageEx*>>& messagesByDateTime);

        // Extract a custom type from the buffer
        template<typename T>
        typename T::UniquePtr_t extractCustomType(ECustomTypeId::Type ctId, int dbId);

        void constructCtQueriesAndHandlers();
        void constructMsgQueriesAndHandlers();
        QList<EMessageId::Type> getSerializableMessages();

        template<typename T>
        std::vector<typename T::UniquePtr_t> deserializeAndGetCustomTypes(ECustomTypeId::Type ctId, const QVariant& encodedIds);

        void pushStandardJpsHeader(DataChunk* jpsFile);
        void readRawStdMessages(const QString& sqlWhere);
        // Moving messages with specified id from _rawMsgBuffer into epochsByDateTime
        void insertRawMessage(const char* msgId, QMap<qulonglong, Epoch*>& epochsByDateTime);
        // Moving messages from collection into epochsByDateTime
        void insertRawMessage(QMap<qulonglong, std::vector<StdMessage*>>& rawMsgs, QMap<qulonglong, Epoch*>& epochsByDateTime);
        // Moving all buffered raw messages into epochsByDateTime
        void insertRawMessage(QMap<qulonglong, Epoch*>& epochsByDateTime);
    private:
        Connection* _connection;
        DatabaseHelper* _dbHelper;

        QString _sqlWhere;

        GreisMysqlSerializer _serializer;

        QMap<ECustomTypeId::Type, QMap<int, CustomType*>> _ctBuffer;
        QMap<ECustomTypeId::Type, QString> _ctQueries;
        QMap<ECustomTypeId::Type, std::function<void(int, const QSqlQuery&, CustomType*&)>> _ctHandlers;
        QMap<EMessageId::Type, QString> _msgQueries;
        QMap<EMessageId::Type, std::function<Message*(const std::string&, int, const QSqlQuery&)>> _msgHandlers;

        QMap<unsigned short, QMap<qulonglong, std::vector<StdMessage*>>> _rawMsgBuffer;

        QMap<int, std::string> _codes;
    };

    template<typename T>
    typename T::UniquePtr_t MySqlSource::extractCustomType( ECustomTypeId::Type ctId, int dbId )
    {
        static_assert(std::is_base_of<CustomType, T>::value, "T must be a CustomType descendant.");

        auto& buffer = _ctBuffer[ctId];
        auto val = buffer.take(dbId);
        if (val != nullptr)
        {
            return typename T::UniquePtr_t(dynamic_cast<T*>(val));
        }

        QString queryStr = _ctQueries.value(ctId);

        assert (queryStr != QString());

        if (!_sqlWhere.isEmpty())
        {
            queryStr +=  " " + _sqlWhere;
        }

        int ctCount = 0;
        typename T::UniquePtr_t retVal;
        QSqlQuery query = _dbHelper->ExecuteQuery(queryStr);
        while (query.next())
        {
            int id = query.value(0).toInt();
            //int idEpoch = query.value(1).toInt();
            //qulonglong unixTime = query.value(2).toULongLong();
            int size = query.value(3).toInt();

            CustomType* ct;
            auto handler = _ctHandlers.value(ctId);

            handler(size, query, ct);

            assert(ct->Size() == size);

            if (id == dbId)
            {
                retVal = typename T::UniquePtr_t(dynamic_cast<T*>(ct));
            } else
            {
                buffer[id] = ct;
            }
            ++ctCount;
        }
        if (ctCount > 0)
        {
            sLogger.Info(QString("%1 custom types readed into memory...").arg(ctCount));
        }

        if (!retVal.get())
        {
            throw DatabaseException(QString("Custom type with id `%1` has not been found in the database.").
                arg(dbId));
        }
        return retVal;
    }

    template<typename T>
    std::vector<typename T::UniquePtr_t> MySqlSource::deserializeAndGetCustomTypes
        ( ECustomTypeId::Type ctId, const QVariant& encodedIds )
    {
        std::vector<int> ids;
        _serializer.DeserializeIds(encodedIds, ids);

        std::vector<typename T::UniquePtr_t> out;
        for (auto it = ids.cbegin(); it != ids.cend(); ++it)
        {
            out.push_back(extractCustomType<T>(ctId, *it));
        }
        return out;
    }
}

#endif // MySqlSource_h__
