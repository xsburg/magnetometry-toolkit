#include "MySqlSink.h"
#include "Common/Connection.h"
#include "AllStdMessages.h"
#include <string>
#include "RawStdMessage.h"

using namespace Common;

namespace Greis
{
    void MySqlSink::construct()
    {
        _lastEpochId = _dbHelper->ExecuteSingleValueQuery(QString("SELECT MAX(`id`) FROM `epoch`")).toInt();
        _epochInserter = DataBatchInserter::SharedPtr_t(new DataBatchInserter(
            "INSERT INTO `epoch` (id, unixTime) VALUES (?, ?)", 2, _connection, "epoch", _inserterBatchSize));

        _rawMessageInserter = DataBatchInserter::SharedPtr_t(new DataBatchInserter(
            "INSERT INTO `rawBinaryMessages` (`idEpoch`, `unixTimeEpoch`, `code`, `bodySize`, `data`) VALUES (?, ?, ?, ?, ?)", 
            5, _connection, "rawBinaryMessages", _inserterBatchSize));
        _rawMessageInserter->AddChild(_epochInserter);

        auto query = _dbHelper->ExecuteQuery("SELECT id, code FROM messageCode");
        while (query.next())
        {
            auto id = query.value(0).toInt();
            auto code = query.value(1).toString();
            std::string codeStr(code.toLatin1().constData(), code.size());
            _codeIds[codeStr] = id;
        }
    }

    MySqlSink::~MySqlSink()
    {
        Flush();
    }

    void MySqlSink::AddEpoch( QDateTime dateTime )
    {
        QVariantList data;
        data << ++_lastEpochId;
        data << dateTime.toMSecsSinceEpoch();
        _lastEpochDateTime = dateTime;
        _index = 0;
        _epochInserter->AddRow(data);
    }

    void MySqlSink::Flush()
    {
        foreach (DataBatchInserter::SharedPtr_t inserter, _msgInserters)
        {
            inserter->Flush();
        }
    }

    void MySqlSink::AddJpsFile( DataChunk* file )
    {
        for (auto epochIt = file->Body().cbegin(); epochIt != file->Body().cend(); ++epochIt)
        {
            AddEpoch((*epochIt)->DateTime);
            for (auto msgIt = (*epochIt)->Messages.cbegin(); msgIt != (*epochIt)->Messages.cend(); ++msgIt)
            {
                AddMessage(msgIt->get());
            }
        }
    }

    void MySqlSink::AddMessage( Message* msg )
    {
        if (msg->Kind() != EMessageKind::StdMessage)
        {
            return;
        }

        auto stdMsg = dynamic_cast<StdMessage*>(msg);

#ifdef _DEBUG
       sLogger.Info(QString("MySqlSink::AddMessage: Id : `%1`, Size : `%2`").arg(stdMsg->Id().c_str()).
            arg(stdMsg->Size()));
#endif
        if (!_codeIds.contains(stdMsg->Id()) || dynamic_cast<RawStdMessage*>(msg) != nullptr)
        {
            QVariantList fields;
            fields << _lastEpochId;
            fields << _index++;
            fields << _lastEpochDateTime.toMSecsSinceEpoch();
            fields << QString::fromLatin1(stdMsg->Id().c_str(), 2);
            fields << stdMsg->BodySize();
            fields << stdMsg->ToByteArray();
            _rawMessageInserter->AddRow(fields);
            return;
        }

        if (!stdMsg->IsCorrect())
        {
            return;
        }

        QVariantList fields;
        fields << _lastEpochId;
        fields << _index++;
        fields << _lastEpochDateTime.toMSecsSinceEpoch();
        fields << _codeIds[stdMsg->Id()];
        fields << stdMsg->BodySize();
        serializeMessage(stdMsg, fields);
        auto inserter = _msgInserters[stdMsg->IdNumber()];
        inserter->AddRow(fields);
    }

    int MySqlSink::addCustomType( CustomType* ct )
    {
        int newId = ++_ctCurrentMaxId[ct->IdNumber()];

        QVariantList fields;
        fields << newId;
        fields << _lastEpochId;
        fields << _lastEpochDateTime.toMSecsSinceEpoch();
        fields << ct->Size();
        serializeCustomType(ct, fields);
        auto inserter = _ctInserters[ct->IdNumber()];
        inserter->AddRow(fields);
        return newId;
    }
}
