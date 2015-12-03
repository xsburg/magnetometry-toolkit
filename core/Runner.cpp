#include "Runner.h"
#include "PosController.h"

core::Runner::Runner()
{
}

void core::Runner::run(RunnerConfig config)
{
    auto webServer = std::make_shared<WebServer>();
    webServer->port(config.webServerPort);

    // Running commands aggregator in background thread
    sLogger.info(QString("Starting web server on port %1...").arg(config.webServerPort));
    webServer->runAsync();
}
