#include <QtCore/QtCore>
#include "common/Logger.h"
#include "common/Path.h"
#include "common/Connection.h"
#include <PosController.h>

using namespace common;

int main(int argc, char** argv)
{
    try
    {
        QCoreApplication a(argc, argv);

        QTextCodec* codec = QTextCodec::codecForName("UTF-8");
        QTextCodec::setCodecForLocale(codec);

        sIniSettings.Initialize(Path::Combine(Path::ApplicationDirPath(), "config.ini"));
        sLogger.initialize(LogLevel(sIniSettings.value("logLevel", LogLevel::Debug).toInt()));

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

        auto runner = std::make_shared<core::PosController>(config);
        runner->run();

        return 0;
    }
    catch (Exception& e)
    {
        sLogger.error(e.what());
        return 1;
    }
}
