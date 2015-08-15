#include "IniSettings.h"
#include "SmartPtr.h"
#include <QtCore/QCoreApplication>
#include <QtCore/QDir>
#include <QtCore/QFile>
#include <QtCore/QTextStream>
#include <QtCore/QRegExp>
#include <QtCore/QMap>
#include <QtCore/QTextCodec>

namespace Common
{
    IniSettings::IniSettings()
    {
    }

    IniSettings::~IniSettings()
    {
    }

    bool IniSettings::Initialize(const QString& settingsFile)
    {
        _settingsFile = settingsFile;
        _pSettings = make_unique<QSettings>(_settingsFile, QSettings::IniFormat);
        _initialized = true;
        return true;
    }

    void IniSettings::setValue( const QString& key, const QVariant& value )
    {
        _pSettings->setValue(key, value);
    }

    QVariant IniSettings::value( const QString & key, const QVariant & defaultValue /*= QVariant()*/ ) const
    {
        return _pSettings->value(key, defaultValue);
    }
}
