#ifndef DataInserter_h__
#define DataInserter_h__

#include <QtCore/QtCore>
#include <boost/utility.hpp>
#include "Exception.h"
#include "Logger.h"
#include "Connection.h"
#include "DatabaseHelper.h"

namespace Common 
{
    class DataInserter : boost::noncopyable
    {
        QString _resultInsertQuery;
        QString _insertTemplate;
        int _rowsAdded;
        Connection* _connection;
        DatabaseHelper* _dbHelper;
        QList<QVariant> _boundValues;
    public:
        // insertTemplate: "INSERT INTO <table name>(<column name>[, <column name>]) VALUES "
        // connection: pointer to connection class
        DataInserter(const QString& insertTemplate, Connection* connection)
        {
            _insertTemplate = insertTemplate;
            _resultInsertQuery = insertTemplate;
            _connection = connection;
            _dbHelper = _connection->DbHelper();
            _rowsAdded = 0;
            _boundValues.clear();
        }

        void AddBindValue(const QVariant& parameterValue)
        {
            _boundValues.append(parameterValue);
        }

        // rowValueInBrackets: "(<value for col1>[, <value for col #>])"
        void AddRow(const QString& rowValueInBrackets)
        {
            int resultLength = _resultInsertQuery.count() + rowValueInBrackets.count() + 10;
            if (resultLength > _connection->MaxAllowedPacket())
            {
                Flush();
            }

            if (_rowsAdded == 0)
            {
                _resultInsertQuery.append(" ");
            } else {
                _resultInsertQuery.append(", ");
            }
            _resultInsertQuery = _resultInsertQuery.append(rowValueInBrackets);
            _rowsAdded++;
        }

        // execute all pending queries
        void Flush()
        {
            if (_rowsAdded > 0)
            {
                QString queryStr = ResultQuery();
                QSqlQuery query = _dbHelper->ExecuteQuery("");
                query.prepare(queryStr);
                DatabaseHelper::ThrowIfError(query);
                foreach (QVariant boundValue, _boundValues)
                {
                    query.addBindValue(boundValue);
                }
                query.exec();
                DatabaseHelper::ThrowIfError(query);

                _resultInsertQuery = _insertTemplate;
                sLogger.Trace(QString("Inserted %1 rows...").arg(_rowsAdded));
                _rowsAdded = 0;
                _boundValues.clear();
            }
        }

        // returns pending query. Throws exception if no row was added till last flush.
        const QString& ResultQuery() const
        {
            if (_rowsAdded == 0)
            {
                throw Exception(QString("Failed to insert, error: %1.").arg(_resultInsertQuery));
            }

            return _resultInsertQuery;
        }
    };
}

#endif // DataInserter_h__