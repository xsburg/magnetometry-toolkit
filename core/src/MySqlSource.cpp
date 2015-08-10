#include "MySqlSource.h"
#include <vector>
#include "Common/SmartPtr.h"
#include "Common/Connection.h"
#include "ECustomTypeId.h"
#include "DataChunk.h"
#include "AllStdMessages.h"
#include "RawStdMessage.h"

using namespace Common;

namespace Greis
{
    MySqlSource::MySqlSource( Connection* connection )
    {
        _connection = connection;
        _dbHelper = _connection->DbHelper();

        auto query = _dbHelper->ExecuteQuery("SELECT id, code FROM messageCode");
        while (query.next())
        {
            auto id = query.value(0).toInt();
            auto code = query.value(1).toString();
            std::string codeStr(code.toLatin1().constData(), code.size());
            _codes[id] = codeStr;
        }

        constructCtQueriesAndHandlers();
        constructMsgQueriesAndHandlers();
    }

    MySqlSource::~MySqlSource()
    {
        for (auto it = _ctBuffer.begin(); it != _ctBuffer.end(); ++it)
        {
            for (auto jt = it->begin(); jt != it->end(); ++jt)
            {
                delete *jt;
                *jt = 0;
            }
        }

        for (auto it = _rawMsgBuffer.begin(); it != _rawMsgBuffer.end(); ++it)
        {
            for (auto jt = it->begin(); jt != it->end(); ++jt)
            {
                for (auto kt = jt->begin(); kt != jt->end(); ++kt)
                {
                    delete *kt;
                    *kt = 0;
                }
            }
        }
    }

    QList<EMessageId::Type> MySqlSource::getSerializableMessages()
    {
        return _msgQueries.keys();
    }

    DataChunk::UniquePtr_t MySqlSource::read(const QString& sqlWhere)
    {
        _ctBuffer.clear();
        _sqlWhere = sqlWhere;

        auto dataChunk = make_unique<DataChunk>();
        pushStandardJpsHeader(dataChunk.get());

        //readRawStdMessages(sqlWhere);

        std::map<qulonglong, std::vector<MessageEx*>> messagesByDateTime;
        for (auto messageId : getSerializableMessages())
        {
            QString query = _msgQueries[messageId] + " " + sqlWhere;
            auto handler = _msgHandlers[messageId];
            handleMessage(query, handler, messagesByDateTime);
        }

        // TODO: 
        //GreisMysqlSerializer& serializer = _serializer;

        // ${HandleMessageStub}
        /*                epoch->DateTime = QDateTime::fromMSecsSinceEpoch(unixTime);

        queryStr = queryStr.arg(_from.toMSecsSinceEpoch()).arg(_to.toMSecsSinceEpoch());
            if (messages == nullptr)
            {
            messages = new QVector<MessageEx>();
            messagesByDateTime[unixTime] = messages;
            }
        messages->push_back(NonStdTextMessage::CreateCarriageReturnMessage());
        messages->push_back(NonStdTextMessage::CreateNewLineMessage()); */

        // TODO:insertRawMessage(epochsByDateTime);

        for (auto it = messagesByDateTime.begin(); it != messagesByDateTime.end(); ++it)
        {
            auto epoch = new Epoch();
            epoch->DateTime = QDateTime::fromMSecsSinceEpoch(it->first);
            auto& messages = it->second;
            std::sort(messages.begin(), messages.end(), [](MessageEx* left, MessageEx* right)
            {
                return left->epochIndex < right->epochIndex;
            });
            for (auto& msgEx : messages)
            {
                epoch->Messages.push_back(std::move(msgEx->msg));
                epoch->Messages.push_back(NonStdTextMessage::CreateCarriageReturnMessage());
                epoch->Messages.push_back(NonStdTextMessage::CreateNewLineMessage());
                delete msgEx;
            }
            dataChunk->Body().push_back(Epoch::UniquePtr_t(epoch));
        }
        return dataChunk;
    }

    void MySqlSource::handleMessage(QString queryStr, std::function<Message*(const std::basic_string<char>&, int, const QSqlQuery&)> handleMessageBody, std::map<qulonglong, std::vector<MessageEx*>>& messagesByDateTime)
    {
        int msgCount = 0;
        QSqlQuery query = _dbHelper->ExecuteQuery(queryStr);
        bool first = true;
        while (query.next())
        {
            int id = query.value(0).toInt();
            //int idEpoch = query.value(1).toInt();
            int epochIndex = query.value(2).toInt();
            qulonglong unixTime = query.value(3).toULongLong();
            int messageCodeId = query.value(4).toInt();
            std::string messageCode = _codes[messageCodeId];
            int bodySize = query.value(5).toInt();

            auto msg = std::unique_ptr<Message>(handleMessageBody(messageCode, bodySize + StdMessage::HeadSize(), query));

            if (!msg->Validate())
            {
                if (first)
                {
                    sLogger.Warn(QString("Message `%1` with Id `%2` has been validated with negative result. It is possible because of floating-point data conversion.").
                        arg(QString::fromLatin1(messageCode.c_str(), 2)).arg(id));
                    first = false;
                }
                if (msg->Kind() == EMessageKind::StdMessage)
                {
                    dynamic_cast<StdMessage*>(msg.get())->RecalculateChecksum();
                    if (!msg->Validate())
                    {
                        throw DataConsistencyException(QString("Invalid message `%1` with Id `%2`.").
                            arg(QString::fromLatin1(messageCode.c_str(), 2)).arg(id));
                    }
                }
            }
            assert(msg->Size() == bodySize + StdMessage::HeadSize());

            messagesByDateTime[unixTime].push_back(new MessageEx(std::move(msg), epochIndex));
            ++msgCount;
        }
        if (msgCount > 0)
        {
            sLogger.Info(QString("%1 messages read into memory...").arg(msgCount));
        }
    }

    DataChunk::UniquePtr_t MySqlSource::ReadAll()
    {
        return this->read();
    }

    DataChunk::UniquePtr_t MySqlSource::ReadRange(const QDateTime& from, const QDateTime& to)
    {
        return this->read(QString("WHERE `unixTimeEpoch` BETWEEN %1 AND %2").arg(from.toMSecsSinceEpoch()).arg(to.toMSecsSinceEpoch()));
    }

    void MySqlSource::pushStandardJpsHeader( DataChunk* dataChunk )
    {
        auto fileId = make_unique<FileIdStdMessage>(
            "JP055RLOGF JPS ALPHA Receiver Log File                                                    ", 90);
        auto msgFmt = make_unique<MsgFmtStdMessage>("MF009JP010109F", 14);
        dataChunk->Head().push_back(std::move(fileId));
        dataChunk->Head().push_back(NonStdTextMessage::CreateCarriageReturnMessage());
        dataChunk->Head().push_back(NonStdTextMessage::CreateNewLineMessage());
        dataChunk->Head().push_back(std::move(msgFmt));
        dataChunk->Head().push_back(NonStdTextMessage::CreateCarriageReturnMessage());
        dataChunk->Head().push_back(NonStdTextMessage::CreateNewLineMessage());
    }

    void MySqlSource::readRawStdMessages(const QString& sqlWhere)
    {
        int msgCount = 0;
        QString command = QString("SELECT `id`, `idEpoch`, `epochIndex`, `unixTimeEpoch`, `code`, `bodySize`, `data` FROM `rawBinaryMessages` %1").arg(sqlWhere);
        QSqlQuery query = _dbHelper->ExecuteQuery(command);
        bool first = true;
        while (query.next())
        {
            int id = query.value(0).toInt();
            int epochIndex = query.value(2).toInt();
            qulonglong unixTime = query.value(3).toULongLong();
            int bodySize = query.value(5).toInt();
            auto data = query.value(6).toByteArray();

            StdMessage* msg = new RawStdMessage(data.data(), data.size());

            assert(msg->Size() == bodySize + StdMessage::HeadSize());

            _rawMsgBuffer[__CHAR2_TO_USHORT(data)][unixTime].push_back(msg);
            ++msgCount;
        }
        sLogger.Info(QString("%1 raw messages readed into memory...").arg(msgCount));
    }

    void MySqlSource::insertRawMessage( const char* msgId, QMap<qulonglong, Epoch*>& epochsByDateTime )
    {
        auto& rawMsgs = _rawMsgBuffer[__CHAR2_TO_USHORT(msgId)];
        insertRawMessage(rawMsgs, epochsByDateTime);
    }

    void MySqlSource::insertRawMessage( QMap<qulonglong, std::vector<StdMessage*>>& rawMsgs, QMap<qulonglong, Epoch*>& epochsByDateTime )
    {
        for (auto it = rawMsgs.begin(); it != rawMsgs.end(); ++it)
        {
            auto epochIt = epochsByDateTime.find(it.key());
            if (epochIt == epochsByDateTime.end())
            {
                auto epoch = new Epoch();
                epoch->DateTime = QDateTime::fromMSecsSinceEpoch(it.key());
                for (auto msgIt = it->begin(); msgIt != it->end(); ++msgIt)
                {
                    epoch->Messages.push_back(Message::UniquePtr_t(*msgIt));
                    (*epochIt)->Messages.push_back(NonStdTextMessage::CreateCarriageReturnMessage());
                    (*epochIt)->Messages.push_back(NonStdTextMessage::CreateNewLineMessage());
                }
                epochsByDateTime[it.key()] = epoch;
            } else {
                for (auto msgIt = it->begin(); msgIt != it->end(); ++msgIt)
                {
                    (*epochIt)->Messages.push_back(Message::UniquePtr_t(*msgIt));
                    (*epochIt)->Messages.push_back(NonStdTextMessage::CreateCarriageReturnMessage());
                    (*epochIt)->Messages.push_back(NonStdTextMessage::CreateNewLineMessage());
                }
            }
        }
        rawMsgs.clear();
    }

    void MySqlSource::insertRawMessage( QMap<qulonglong, Epoch*>& epochsByDateTime )
    {
        for (auto it = _rawMsgBuffer.begin(); it != _rawMsgBuffer.end(); ++it)
        {
            insertRawMessage(it.value(), epochsByDateTime);
        }
    }
}
