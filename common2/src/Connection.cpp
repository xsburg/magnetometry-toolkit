#include "Connection.h"
#include "SmartPtr.h"

namespace Common
{
    void Connection::Connect()
    {
        if (_db.isOpen())
            return;

        QString escapedHostname = Hostname;
        escapedHostname.replace('.', '_');
        QString connectionName = QUuid::createUuid().toString();
        _db = QSqlDatabase::addDatabase(Driver, connectionName);
        _db.setDatabaseName(DatabaseName);
        _db.setPort(Port);
        _db.setHostName(Hostname);
        _db.setUserName(Username);
        _db.setPassword(Password);
        _db.setConnectOptions(QString("CLIENT_COMPRESS=1"));
        //_db.setConnectOptions(QString("useCompression=true"));
        if (!_db.open())
        {
            throw DatabaseException(_db.lastError().text());
        }

        // ѕолучение максимального размера пакета
        _dbHelper = make_unique<DatabaseHelper>(_db);
        _maxAllowedPacket = _dbHelper->ExecuteSingleRowQuery(
            "SHOW VARIABLES LIKE 'max_allowed_packet'").value(1).toInt();
    }

    Connection::SharedPtr_t Connection::FromSettings(const QString& settingsPrefix)
    {
        Connection::SharedPtr_t ci = std::make_shared<Connection>();
        ci->_settingsPrefix = settingsPrefix;
        ci->Driver = sIniSettings.value(settingsPrefix + ".Driver", "").toString();
        ci->DatabaseName = sIniSettings.value(settingsPrefix + ".DatabaseName", "").toString();
        ci->Username = sIniSettings.value(settingsPrefix + ".Username", "").toString();
        ci->Password = sIniSettings.value(settingsPrefix + ".Password", "").toString();
        ci->Hostname = sIniSettings.value(settingsPrefix + ".Hostname", "").toString();
        ci->Port = sIniSettings.value(settingsPrefix + ".Port", 0).toInt();
        return ci;
    }

    void Connection::ToSettings()
    {
        sIniSettings.setValue(_settingsPrefix + ".Driver", Driver);
        sIniSettings.setValue(_settingsPrefix + ".DatabaseName", DatabaseName);
        sIniSettings.setValue(_settingsPrefix + ".Username", Username);
        sIniSettings.setValue(_settingsPrefix + ".Password", Password);
        sIniSettings.setValue(_settingsPrefix + ".Hostname", Hostname);
        sIniSettings.setValue(_settingsPrefix + ".Port", Port);
    }
}
