#include "Runner.h"
#include "PosController.h"

core::Runner::Runner()
{
}

void core::Runner::run(RunnerConfig config)
{
    // Creating output miniseed stream
    auto msSink = std::make_shared<MSeedSink>(config.msSettings.fileName);

    // Starting web server
    auto webServer = std::make_shared<WebServer>();
    webServer->port(config.webServerPort);
    sLogger.info(QString("Starting web server on port %1...").arg(config.webServerPort));
    webServer->runAsync();

    // Starting devices
    auto posController = std::make_shared<PosController>(webServer, msSink, config.devicePortName, config.msSettings);
    posController->runAsync();

    // Sleeping the main thread
    while (true)
    {
        QThread::currentThread()->sleep(5);
    }
}
