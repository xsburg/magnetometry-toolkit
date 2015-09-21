#include "RunnerActionHandler.h"
#include <mongoose.h>
#include <common/InvalidOperationException.h>
#include "RunnerCommands.h"

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
    auto intervalMilliseconds = json.value("intervalMilliseconds").toInt();
    QMutexLocker lock(dataMutex());
    _commands.enqueue(std::make_shared<RunRunnerCommand>(intervalMilliseconds));
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
        QJsonDocument document;
        QJsonObject json;
        json["about"] = _status->about;
        json["enq"] = _status->enq;
        json["time"] = _status->time.toString(Qt::ISODate);
        json["updated"] = _status->updated.toString(Qt::ISODate);
        json["standBy"] = _status->standBy;
        QJsonObject range;
        range["minField"] = _status->range.minField;
        range["maxField"] = _status->range.maxField;
        json["range"] = range;

        document.setObject(json);
        auto jsonData = document.toJson(QJsonDocument::JsonFormat::Indented);

        mg_write(connection(), jsonData.data(), jsonData.size());
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
        else
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
