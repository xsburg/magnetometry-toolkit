#include "Runner.h"
#include <mongoose.h>

bool core::RunnerActionHandler::match()
{
    /*
    GET api/status
    POST api/command
    */
    return
        (exactMatch("api/status") && methodMatch("GET")) ||
        (exactMatch("api/command") && methodMatch("POST"));
}

void core::RunnerActionHandler::execute()
{
    if (exactMatch("api/status"))
    {
        // api/status
        mg_printf_data(connection(), "{ \"result\": \"Some Test Data\" }");
    }
    else if (exactMatch("api/command"))
    {
        // api/command
        mg_printf_data(connection(), "{ \"result\": \"Some Test Data\" }");
    }
}

core::Runner::Runner(RunnerConfig config)
{
    _actionHandler = std::make_shared<RunnerActionHandler>();
    _webServer = std::make_shared<WebServer>();
    _webServer->port(config.webServerPort);
    _webServer->addActionHandler(_actionHandler);
}

void core::Runner::run()
{
    _webServer->runAsync();
}
