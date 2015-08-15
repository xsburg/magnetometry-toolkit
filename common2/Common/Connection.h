#pragma once

#include <QtCore/QtCore>
#include <QtSql/QtSql>
#include <boost/utility.hpp>
#include "DatabaseException.h"
#include "DatabaseHelper.h"
#include "IniSettings.h"
#include "Logger.h"

namespace Common 
{
    class Connection
    {
    private:
        QSqlDatabase _db;
        QString _settingsPrefix;
        int _maxAllowedPacket;
        DatabaseHelper::UniquePtr_t _dbHelper;
    public:
        SMART_PTR_T(Connection);

        QString Driver;
        QString DatabaseName;
        QString Username;
        QString Password;
        QString Hostname;
        int Port;
    
        Connection()
        {
        };

        void Connect();
    
        inline QSqlDatabase& Database() { return _db; }
        inline DatabaseHelper* DbHelper() { return _dbHelper.get(); }
        inline int MaxAllowedPacket() const { return _maxAllowedPacket; }
    
        static Connection::SharedPtr_t FromSettings(const QString& settingsPrefix);
        void ToSettings();
    private:
        Connection(const Connection& con);
        Connection &operator =(const Connection &);
    };
}
