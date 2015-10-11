﻿#include "RunnerActionHandler.h"
#include <mongoose.h>
#include <common/InvalidOperationException.h>
#include "RunnerCommands.h"

core::RunnerActionHandler::RunnerActionHandler() : _status(new RunnerStatus()), _logger(new BufferedLogger())
{
}

bool core::RunnerActionHandler::match()
{
    /*
    GET api/status
    POST api/command
    */
    return
        (exactMatch("api/status") && methodMatch("GET")) ||
        (exactMatch("api/log") && methodMatch("GET")) ||
        (exactMatch("api/command") && methodMatch("POST"));
}

void core::RunnerActionHandler::addRunCommand(QJsonObject json)
{
    auto intervalMilliseconds = json.value("intervalMilliseconds").toInt();
    _commands.enqueue(std::make_shared<RunRunnerCommand>(intervalMilliseconds));
}

void core::RunnerActionHandler::addStopCommand(QJsonObject json)
{
    _commands.enqueue(std::make_shared<StopRunnerCommand>());
}

void core::RunnerActionHandler::addUpdateStatusCommand(QJsonObject json)
{
    _commands.enqueue(std::make_shared<UpdateStatusRunnerCommand>());
}

void core::RunnerActionHandler::addSetTimeCommand(QJsonObject json)
{
    auto unixTime = json.value("time").toInt();
    auto time = QDateTime::fromTime_t(unixTime, Qt::UTC);
    _commands.enqueue(std::make_shared<SetTimeRunnerCommand>(time));
}

void core::RunnerActionHandler::addSetRangeCommand(QJsonObject json)
{
    auto range = json.value("range").toInt();
    _commands.enqueue(std::make_shared<SetRangeRunnerCommand>(range));
}

void core::RunnerActionHandler::addSetStandByCommand(QJsonObject json)
{
    auto standBy = json.value("standBy").toBool();
    _commands.enqueue(std::make_shared<SetStandByRunnerCommand>(standBy));
}

void core::RunnerActionHandler::addRunDiagnosticsCommand(QJsonObject json)
{
    _commands.enqueue(std::make_shared<RunDiagnosticsRunnerCommand>());
}

void core::RunnerActionHandler::addRunModeAutoTestCommand(QJsonObject json)
{
    _commands.enqueue(std::make_shared<RunModeAutoTestRunnerCommand>());
}

void core::RunnerActionHandler::execute()
{
    QMutexLocker lock(dataMutex());
    if (exactMatch("api/status"))
    {
        // api/status
        QJsonDocument document;
        QJsonObject json;
        json["about"] = _status->about;
        json["enq"] = _status->enq;
        json["time"] = _status->time.toString(Qt::ISODate);
        json["timeUpdated"] = _status->timeUpdated.toString(Qt::ISODate);
        json["updated"] = _status->updated.toString(Qt::ISODate);
        json["standBy"] = _status->standBy;
        json["isRunning"] = _status->isRunning;
        json["samplingIntervalMs"] = _status->samplingIntervalMs;
        QJsonObject range;
        range["minField"] = _status->range.minField;
        range["maxField"] = _status->range.maxField;
        json["range"] = range;
        json["commandQueueSize"] = _status->commandQueueSize;

        document.setObject(json);
        auto jsonData = document.toJson(QJsonDocument::JsonFormat::Indented);

        mg_send_data(connection(), jsonData.data(), jsonData.size());
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
        }
        else if (command == "run-diagnostics")
        {
            addRunDiagnosticsCommand(root);
        }
        else if (command == "run-mode-auto-test")
        {
            addRunModeAutoTestCommand(root);
        }
        else
        {
            throw Common::InvalidOperationException(QString("Command `%1` is not supported.").arg(command));
        }

        _status->commandQueueSize++;
        mg_printf_data(connection(), "{ \"result\": \"enqueued\" }");
    }
    else if (exactMatch("api/log"))
    {
        // api/log
        QJsonDocument document;
        QJsonObject json;
        QJsonArray messages;

        for (auto& msg : _logger->buffer())
        {
            QJsonObject messageObject;
            messageObject["time"] = msg.time.toString(Qt::ISODate);
            messageObject["id"] = qint64(msg.id);
            messageObject["logLevel"] = msg.logLevel;
            messageObject["message"] = msg.message;
            messages.append(messageObject);
        }

        json["messages"] = messages;
        document.setObject(json);
        auto jsonData = document.toJson(QJsonDocument::JsonFormat::Indented);

        mg_send_data(connection(), jsonData.data(), jsonData.size());
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
