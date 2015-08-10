#ifndef MySqlSink_h__
#define MySqlSink_h__

#include <boost/noncopyable.hpp>
#include <QtCore/QList>
#include <QtCore/QMap>
#include "Common/Connection.h"
#include "Common/SmartPtr.h"
#include "Common/NotSupportedException.h"
#include "Common/NotImplementedException.h"
#include "Common/DataInserter.h"
#include "Common/DataBatchInserter.h"
#include "EMessageId.h"
#include "ECustomTypeId.h"
#include "DataChunk.h"
#include "GreisMysqlSerializer.h"
#include "CustomType.h"

using namespace Common;

namespace Greis
{
    class MySqlSink : private boost::noncopyable
    {
    public:
        SMART_PTR_T(MySqlSink);

        MySqlSink(Connection* connection, int inserterBatchSize = 10000);
        ~MySqlSink();

        void AddJpsFile(DataChunk* file);

        void AddEpoch(QDateTime dateTime);
        void AddMessage(Message* msg);

        void Flush();
    private:
        void construct();

        int addCustomType(CustomType* ct);

        // Serialize objects fields into QVariantList with calling addCustomType if needed.
        void serializeMessage(StdMessage* msg, QVariantList& out);
        void serializeCustomType(CustomType* ct, QVariantList& out);

        // Add vector of custom types and serialize it for standard message table field.
        template<typename T>
        inline QVariant addCustomTypesAndSerialize(const std::vector<T>& field)
        {
            std::vector<int> ids;
            for (auto it = field.cbegin(); it != field.cend(); ++it)
            {
                int idVal = addCustomType(it->get());
                ids.push_back(idVal);
            }
            return _serializer.SerializeIds(ids);
        }
    private:
        Connection* _connection;
        DatabaseHelper* _dbHelper;
        int _inserterBatchSize;

        QMap<EMessageId::Type, DataBatchInserter::SharedPtr_t> _msgInserters;
        QMap<ECustomTypeId::Type, DataBatchInserter::SharedPtr_t> _ctInserters;

        GreisMysqlSerializer _serializer;

        DataBatchInserter::SharedPtr_t _epochInserter;
        DataBatchInserter::SharedPtr_t _rawMessageInserter;
        int _lastEpochId;
        int _index;
        QDateTime _lastEpochDateTime;

        QMap<ECustomTypeId::Type, int> _ctCurrentMaxId;
        QMap<std::string, int> _codeIds;
    };
}

#endif // MySqlSink_h__