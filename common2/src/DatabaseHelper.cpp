#include "DatabaseHelper.h"
#include "DatabaseException.h"

namespace Common
{
    void DatabaseHelper::ThrowIfError( const QSqlQuery& query )
    {
        if (query.lastError().isValid())
        {
            auto text = query.lastError().text();
            throw DatabaseException(text);
        }
    }

    QSqlQuery DatabaseHelper::ExecuteQuery(const QString& queryString, QSqlDatabase db/* = QSqlDatabase()*/)
    {
        QSqlQuery query(queryString, db);
        ThrowIfError(query);
        return query;
    }

    QSqlQuery DatabaseHelper::ExecuteQuery(const QString& queryString, const QList<QVariant>& bindValues, 
                                           QSqlDatabase db/* = QSqlDatabase()*/)
    {
        QSqlQuery query(db);
        query.prepare(queryString);
        ThrowIfError(query);
        foreach (QVariant val, bindValues)
        {
            query.addBindValue(val);
        }
        query.exec();
        ThrowIfError(query);
        return query;
    }

    QSqlQuery DatabaseHelper::ExecuteSingleRowQuery(const QString& queryString, QSqlDatabase db/* = QSqlDatabase()*/)
    {
        QSqlQuery query = ExecuteQuery(queryString, db);
        if (!query.next())
        {
            throw DatabaseException(QString("The query '%1' has returned 0 records. One record was expected.").
                arg(queryString));
        }
        return query;
    }

    QSqlQuery DatabaseHelper::ExecuteSingleRowQuery(const QString& queryString, const QList<QVariant>& bindValues, 
                                                    QSqlDatabase db/* = QSqlDatabase()*/)
    {
        QSqlQuery query = ExecuteQuery(queryString, bindValues, db);
        if (!query.next())
        {
            throw DatabaseException(QString("The query '%1' has returned 0 records. One record was expected.").
                arg(queryString));
        }
        return query;
    }

    QVariant DatabaseHelper::ExecuteSingleValueQuery(const QString& queryString, QSqlDatabase db/* = QSqlDatabase()*/)
    {
        QSqlQuery query = ExecuteSingleRowQuery(queryString, db);
        QVariant value = query.value(0);
        if (!value.isValid())
        {
            throw DatabaseException(QString("The query '%1' has returned invalid value for index 0.").arg(queryString));
        }
        return value;
    }

    QVariant DatabaseHelper::ExecuteSingleValueQuery(const QString& queryString, const QList<QVariant>& bindValues, 
                                                     QSqlDatabase db/* = QSqlDatabase()*/)
    {
        QSqlQuery query = ExecuteSingleRowQuery(queryString, bindValues, db);
        QVariant value = query.value(0);
        if (!value.isValid())
        {
            throw DatabaseException(QString("The query '%1' has returned invalid value for index 0.").arg(queryString));
        }
        return value;
    }
}
