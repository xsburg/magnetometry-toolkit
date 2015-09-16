#include <QtCore/QtCore>
#include "common/Logger.h"
#include "common/SmartPtr.h"
#include "common/Path.h"
#include "common/Connection.h"
#include <boost/system/system_error.hpp>
#include <EbDeviceManager.h>
#include <boost/exception/diagnostic_information.hpp>

using namespace Common;

int main(int argc, char** argv)
{
    try
    {
        QCoreApplication a(argc, argv);

        QTextCodec* codec = QTextCodec::codecForName("UTF-8");
        QTextCodec::setCodecForLocale(codec);

        sIniSettings.Initialize(Path::Combine(Path::ApplicationDirPath(), "config.ini"));
        sLogger.Initialize(sIniSettings.value("LogLevel", 5).toInt());

        sLogger.Debug("The following sqldrivers are available:");
        auto sqlDrivers = QSqlDatabase::drivers();
        for (auto it = sqlDrivers.begin(); it != sqlDrivers.end(); ++it)
        {
            QString sqlDriverName = *it;
            sLogger.Debug(sqlDriverName);
        }

        auto portName = sIniSettings.value("device/portName").toString();

        sLogger.Info("=== Config ===");
        sLogger.Info(QString("device.portName: %1").arg(portName));
        sLogger.Info("==============");

        auto device = std::make_shared<core::EbDeviceManager>();
        device->connect(portName);
        device->runDiagnosticSequence();






        return 0;
    }
    catch (boost::system::system_error& ex)
    {
        sLogger.Error(QString("Error: %1").arg(ex.what()));
        auto info = boost::diagnostic_information(ex);
        sLogger.Error(QString("Info: %1").arg(info.c_str()));
        if (ex.code().value() == 2) {
            sLogger.Error("Connection to receiver could not be made.");
            sLogger.Error("The application could not find the port specified.");
            sLogger.Error("It is ether wrong configuration or the receiver moved to another port.");
        }
        else {
            sLogger.Error("No connection to receiver, code: " + QString::number(ex.code().value()));
        }
        return 1;
    }
    catch (Exception& e)
    {
        sLogger.Error(e.what());
        return 1;
    }
}
