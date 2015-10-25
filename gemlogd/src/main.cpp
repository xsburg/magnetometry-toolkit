#include <QtCore/QtCore>
#include "common/Logger.h"
#include "common/SmartPtr.h"
#include "common/Path.h"
#include "common/Connection.h"
#include <boost/system/system_error.hpp>
#include <EbDevice.h>
#include <boost/exception/diagnostic_information.hpp>
#include <Runner.h>

using namespace common;

int main(int argc, char** argv)
{
    try
    {
        QCoreApplication a(argc, argv);

        QTextCodec* codec = QTextCodec::codecForName("UTF-8");
        QTextCodec::setCodecForLocale(codec);

        sIniSettings.Initialize(Path::Combine(Path::ApplicationDirPath(), "config.ini"));
        sLogger.initialize((LogLevel)sIniSettings.value("logLevel", 5).toInt());

        sLogger.debug("The following sqldrivers are available:");
        auto sqlDrivers = QSqlDatabase::drivers();
        for (auto it = sqlDrivers.begin(); it != sqlDrivers.end(); ++it)
        {
            QString sqlDriverName = *it;
            sLogger.debug(sqlDriverName);
        }

        auto portName = sIniSettings.value("device/portName").toString();

        sLogger.info("=== Config ===");
        sLogger.info(QString("device.portName: %1").arg(portName));
        sLogger.info("==============");

        core::RunnerConfig config;
        config.webServerPort = sIniSettings.value("webServer/port", 8000).toInt();
        config.devicePortName = sIniSettings.value("device/portName").toString();
        config.msRecordLocation = sIniSettings.value("mseed/location").toString();
        config.msRecordNetwork = sIniSettings.value("mseed/network").toString();
        config.msRecordStation = sIniSettings.value("mseed/station").toString();
        config.msFileName = sIniSettings.value("mseed/fileName").toString();
        config.samplesCacheMaxSize = sIniSettings.value("runner/samplesCacheMaxSize", 100).toInt();
        config.skipDiagnostics = sIniSettings.value("runner/skipDiagnostics", false).toBool();

        auto runner = std::make_shared<core::Runner>(config);
        runner->run();

        return 0;
    }
    catch (boost::system::system_error& ex)
    {
        sLogger.error(QString("Error: %1").arg(ex.what()));
        auto info = boost::diagnostic_information(ex);
        sLogger.error(QString("Info: %1").arg(info.c_str()));
        if (ex.code().value() == 2) {
            sLogger.error("Connection to receiver could not be made.");
            sLogger.error("The application could not find the port specified.");
            sLogger.error("It is ether wrong configuration or the receiver moved to another port.");
        }
        else
        {
            sLogger.error("No connection to receiver, code: " + QString::number(ex.code().value()));
        }
        return 1;
    }
    catch (Exception& e)
    {
        sLogger.error(e.what());
        return 1;
    }
}
