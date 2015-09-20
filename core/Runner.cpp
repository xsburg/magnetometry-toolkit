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

void core::RunnerActionHandler::executeRunCommand(QJsonObject json)
{
    auto interval = json.value("interval").toDouble();
    auto active = json.value("active").toBool();

    {
        auto mutex = lockSharedData();
        _commands.push(command);
    }

    mg_printf_data(connection(), "{ \"result\": \"enqueued\" }");
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
        QString content(QByteArray(connection()->content, connection()->content_len));
        QJsonDocument doc = QJsonDocument::fromJson(content.toLatin1());
        auto root = doc.object();
        auto command = root.value("command").toString();
        if (command == "run")
        {
            executeRunCommand(root);
        }
        else if (command == "stop")
        {
            executeStopCommand(root);
        }
        else if (command == "update-status")
        {
            executeUpdateStatusCommand(root);
        }
        else if (command == "set-device-time")
        {
            executeSetTimeCommand(root);
        }
        else if (command == "set-device-range")
        {
            executeSetRangeCommand(root);
        }
        else if (command == "set-device-stand-by")
        {
            executeSetStandByCommand(root);
        }
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
