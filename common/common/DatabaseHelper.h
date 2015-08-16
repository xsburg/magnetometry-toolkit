#ifndef ExceptionHelper_h__
#define ExceptionHelper_h__

#include <QtCore/QtCore>
#include <QtSql/QtSql>
#include <boost/utility.hpp>
#include "SmartPtr.h"

namespace Common 
{
    class DatabaseHelper : boost::noncopyable
    {
    private:
        QSqlDatabase _db;
    public:
        SMART_PTR_T(DatabaseHelper);

        DatabaseHelper(const QSqlDatabase& db) : _db(db)
        {
        }

        static void ThrowIfError(const QSqlQuery& query);

        inline QSqlQuery CreateQuery()
        {
            return QSqlQuery(_db);
        }

        inline QSqlQuery ExecuteQuery(const QString& queryString)
        {
            return ExecuteQuery(queryString, _db);
        }

        // Executing query with query string `queryString` and unnamed input bind values `bindValues`.
        inline QSqlQuery ExecuteQuery(const QString& queryString, const QList<QVariant>& bindValues)
        {
            return ExecuteQuery(queryString, bindValues, _db);
        }

        static QSqlQuery ExecuteQuery(const QString& queryString, QSqlDatabase db/* = QSqlDatabase()*/);

        // Executing query with database `db`, query string `queryString` and unnamed input bind values `bindValues`.
        static QSqlQuery ExecuteQuery(const QString& queryString, const QList<QVariant>& bindValues, 
                                      QSqlDatabase db/* = QSqlDatabase()*/);

        inline QSqlQuery ExecuteSingleRowQuery(const QString& queryString)
        {
            return ExecuteSingleRowQuery(queryString, _db);
        }

        inline QSqlQuery ExecuteSingleRowQuery(const QString& queryString, const QList<QVariant>& bindValues)
        {
            return ExecuteSingleRowQuery(queryString, bindValues, _db);
        }

        static QSqlQuery ExecuteSingleRowQuery(const QString& queryString, QSqlDatabase db/* = QSqlDatabase()*/);

        static QSqlQuery ExecuteSingleRowQuery(const QString& queryString, const QList<QVariant>& bindValues, 
                                               QSqlDatabase db/* = QSqlDatabase()*/);

        inline QVariant ExecuteSingleValueQuery(const QString& queryString)
        {
            return ExecuteSingleValueQuery(queryString, _db);
        }

        inline QVariant ExecuteSingleValueQuery(const QString& queryString, const QList<QVariant>& bindValues)
        {
            return ExecuteSingleValueQuery(queryString, bindValues, _db);
        }

        static QVariant ExecuteSingleValueQuery(const QString& queryString, QSqlDatabase db/* = QSqlDatabase()*/);

        static QVariant ExecuteSingleValueQuery(const QString& queryString, const QList<QVariant>& bindValues, 
                                                QSqlDatabase db/* = QSqlDatabase()*/);
    };
}

#endif // ExceptionHelper_h__
