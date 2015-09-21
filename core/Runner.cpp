#include "Runner.h"
#include <mongoose.h>
#include <common/InvalidOperationException.h>

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

void core::RunnerActionHandler::addRunCommand(QJsonObject json)
{
    QMutexLocker lock(dataMutex());
    _commands.enqueue(std::make_shared<RunRunnerCommand>());
}

void core::RunnerActionHandler::addStopCommand(QJsonObject json)
{
    QMutexLocker lock(dataMutex());
    _commands.enqueue(std::make_shared<StopRunnerCommand>());
}

void core::RunnerActionHandler::addUpdateStatusCommand(QJsonObject json)
{
    QMutexLocker lock(dataMutex());
    _commands.enqueue(std::make_shared<UpdateStatusRunnerCommand>());
}

void core::RunnerActionHandler::addSetTimeCommand(QJsonObject json)
{
    auto unixTime = json.value("time").toInt();
    auto time = QDateTime::fromTime_t(unixTime, Qt::UTC);
    QMutexLocker lock(dataMutex());
    _commands.enqueue(std::make_shared<SetTimeRunnerCommand>(time));
}

void core::RunnerActionHandler::addSetRangeCommand(QJsonObject json)
{
    auto range = json.value("range").toInt();
    QMutexLocker lock(dataMutex());
    _commands.enqueue(std::make_shared<SetRangeRunnerCommand>(range));
}

void core::RunnerActionHandler::addSetStandByCommand(QJsonObject json)
{
    auto standBy = json.value("standBy").toBool();
    QMutexLocker lock(dataMutex());
    _commands.enqueue(std::make_shared<SetStandByRunnerCommand>(standBy));
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
            addRunCommand(root);
        }
        else if (command == "stop")
        {
            addStopCommand(root);
        }
        else if (command == "update-status")
        {
            addUpdateStatusCommand(root);
        }
        else if (command == "set-device-time")
        {
            addSetTimeCommand(root);
        }
        else if (command == "set-device-range")
        {
            addSetRangeCommand(root);
        }
        else if (command == "set-device-stand-by")
        {
            addSetStandByCommand(root);
        } else
        {
            throw Common::InvalidOperationException();
        }
        mg_printf_data(connection(), "{ \"result\": \"enqueued\" }");
    }
    else
    {
        throw Common::InvalidOperationException();
    }
}

QMutex* core::RunnerActionHandler::dataMutex()
{
    return &_dataMutex;
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
