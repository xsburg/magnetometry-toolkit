#ifndef IniSettings_h__
#define IniSettings_h__

#include <QtCore/QSettings>
#include <QtCore/QString>
#include <memory>
#include <boost/utility.hpp>
#include "Singleton.h"

namespace Common
{
    class IniSettings : boost::noncopyable
    {
        SINGLETON_BLOCK(IniSettings)
    public:
        IniSettings();
        ~IniSettings();

        bool Initialize(const QString& settingsFile);
        void setValue(const QString& key, const QVariant& value);
        QVariant value(const QString & key, const QVariant & defaultValue = QVariant()) const;

        inline QSettings* settings() { return _pSettings.get(); }
    private:
        std::unique_ptr<QSettings> _pSettings;
        QString _settingsFile;
        bool _initialized;
    };
}

#define sIniSettings SINGLETON_INSTANCE(::Common::IniSettings)

#endif // IniSettings_h__
