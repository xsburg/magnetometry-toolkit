#include "DataBatchInserter.h"
#include "Logger.h"

namespace Common
{
    DataBatchInserter::DataBatchInserter(const QString& insertQuery, int boundColumnsCount, Connection* connection, 
                                         const QString& tableName, int batchSize)
    {
        _insertQuery = insertQuery;
        _connection = connection;
        _dbHelper = _connection->DbHelper();
        _rowsAdded = 0;
        _batchSize = batchSize;
        _boundValues.resize(boundColumnsCount);
        _tableName = tableName;
    }

    DataBatchInserter::~DataBatchInserter()
    {
        Flush();
    }

    void DataBatchInserter::AddChild(DataBatchInserter::SharedPtr_t child)
    {
        _children.push_back(child);
    }

    void DataBatchInserter::AddRow(const QList<QVariant>& values)
    {
        if (values.size() != _boundValues.size())
        {
            throw InvalidOperationException(
                QString("Invalid elements count in `AddRow(values)` array. Actual: %1. Expected: %2.").
                arg(values.size()).arg(_boundValues.size()));
        }
        if (_rowsAdded >= _batchSize)
        {
            Flush();
        }

        for (int i = 0; i < _boundValues.size(); ++i)
        {
            _boundValues[i].append(values.at(i));
        }
        _rowsAdded++;
    }

    void DataBatchInserter::Flush()
    {
        if (_rowsAdded > 0)
        {
            foreach (DataBatchInserter::SharedPtr_t child, _children)
            {
                child->Flush();
            }

            auto insertQueryTmp = _insertQuery;
            auto valuesTmp = QString(",(?");
            for (int i = 1; i < _boundValues.size(); i++)
            {
                valuesTmp.append(",?");
            }
            valuesTmp.append(")");

            for (int i = 1; i < _rowsAdded; i++)
            {
                insertQueryTmp.append(valuesTmp);
            }

            QSqlQuery query = _dbHelper->ExecuteQuery("");
            //sLogger.Debug(insertQueryTmp);
            query.prepare(insertQueryTmp);
            DatabaseHelper::ThrowIfError(query);
            int varsCount = 0;

            auto colSize = _boundValues.size();
            for (int i = 0; i < _rowsAdded; i++)
            {
                for (int j = 0; j < colSize; j++)
                {
                    query.addBindValue(_boundValues[j][i]);
                    varsCount++;
                }
            }

            auto valid = varsCount == _boundValues.size() * _rowsAdded;
            query.exec();
            DatabaseHelper::ThrowIfError(query);

            if (_tableName.isEmpty() || _tableName.isNull())
            {
                sLogger.Trace(QString("%1 records has been added.").arg(_rowsAdded));
            } else {
                sLogger.Trace(QString("%1 records has been added into `%2`.").arg(_rowsAdded).arg(_tableName));
            }
            _rowsAdded = 0;
            int size = _boundValues.size();
            _boundValues.clear();
            _boundValues.resize(size);
        }
    }
}
