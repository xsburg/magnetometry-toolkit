#include "PosWebHandler.h"
#include <mongoose/mongoose.h>
#include <common/InvalidOperationException.h>
#include <common/Helpers.h>
#include "PosWebCommands.h"

core::PosWebHandler::PosWebHandler() : _status(new PosStatus()), _logger(new BufferedLogger())
{
}

bool core::PosWebHandler::match()
{
    /*
    GET api/pos/status
    POST api/pos/command
    */
    return
        (exactMatch("api/status") && methodMatch("GET")) ||
        (exactMatch("api/log") && methodMatch("GET")) ||
        (exactMatch("api/data") && methodMatch("GET")) ||
        (exactMatch("api/command") && methodMatch("POST"));
}

void core::PosWebHandler::addToDataBuffer(const PosDevice::Sample& sample)
{
    dataSamples.append(sample);
    while (dataSamples.size() > maxDataSamplesListSize)
    {
        dataSamples.pop_front();
    }
}

void core::PosWebHandler::addRunCommand(QJsonObject json)
{
    auto intervalMilliseconds = json.value("intervalMilliseconds").toInt();
    auto timeFixIntervalSeconds = json.value("timeFixIntervalSeconds").toInt();
    _commands.enqueue(std::make_shared<PosRunWebCommand>(intervalMilliseconds, timeFixIntervalSeconds));
}

void core::PosWebHandler::addStopCommand(QJsonObject json)
{
    _commands.enqueue(std::make_shared<PosStopWebCommand>());
}

void core::PosWebHandler::addUpdateStatusCommand(QJsonObject json)
{
    _commands.enqueue(std::make_shared<PosUpdateStatusWebCommand>());
}

void core::PosWebHandler::addSetTimeCommand(QJsonObject json)
{
    auto unixTime = json.value("time").toInt();
    auto time = QDateTime::fromTime_t(unixTime, Qt::UTC);
    _commands.enqueue(std::make_shared<PosSetTimeWebCommand>(time));
}

void core::PosWebHandler::addSetRangeCommand(QJsonObject json)
{
    auto range = json.value("range").toInt();
    _commands.enqueue(std::make_shared<PosSetRangeWebCommand>(range));
}

void core::PosWebHandler::addSetStandByCommand(QJsonObject json)
{
    auto standBy = json.value("standBy").toBool();
    _commands.enqueue(std::make_shared<PosSetStandByWebCommand>(standBy));
}

void core::PosWebHandler::addRunDiagnosticsCommand(QJsonObject json)
{
    _commands.enqueue(std::make_shared<PosRunDiagnosticsWebCommand>());
}

void core::PosWebHandler::addRunModeAutoTestCommand(QJsonObject json)
{
    _commands.enqueue(std::make_shared<PosRunModeAutoTestWebCommand>());
}

void core::PosWebHandler::addApplyMSeedSettingsCommand(QJsonObject json)
{
    MSeedSettings settings;
    settings.fileName = json.value("fileName").toString();
    settings.location = json.value("location").toString();
    settings.network = json.value("network").toString();
    settings.station = json.value("station").toString();
    settings.samplesInRecord = json.value("samplesInRecord").toInt();
    _commands.enqueue(std::make_shared<PosApplyMSeedSettingsWebCommand>(settings));
}

void core::PosWebHandler::execute()
{
    QMutexLocker lock(dataMutex());
    if (exactMatch("api/status"))
    {
        // api/status
        QJsonDocument document;
        QJsonObject json;
        json["about"] = _status->about;
        json["enq"] = _status->enq;
        json["time"] = common::Helpers::toISODateWithMilliseconds(_status->time);
        json["timeUpdated"] = common::Helpers::toISODateWithMilliseconds(_status->timeUpdated);
        json["updated"] = common::Helpers::toISODateWithMilliseconds(_status->updated);
        json["standBy"] = _status->standBy;
        json["isRunning"] = _status->isRunning;
        json["samplingIntervalMs"] = _status->samplingIntervalMs;
        json["timeFixIntervalSeconds"] = _status->timeFixIntervalSeconds;
        
        QJsonObject range;
        range["minField"] = _status->range.minField;
        range["maxField"] = _status->range.maxField;
        json["range"] = range;
        
        QJsonObject mseedSettings;
        mseedSettings["fileName"] = _status->mseedSettings.fileName;
        mseedSettings["location"] = _status->mseedSettings.location;
        mseedSettings["network"] = _status->mseedSettings.network;
        mseedSettings["station"] = _status->mseedSettings.station;
        mseedSettings["samplesInRecord"] = _status->mseedSettings.samplesInRecord;
        json["mseedSettings"] = mseedSettings;

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
        else if (command == "apply-mseed-settings")
        {
            addApplyMSeedSettingsCommand(root);
        }
        else
        {
            throw common::InvalidOperationException(QString("Command `%1` is not supported.").arg(command));
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
            messageObject["time"] = common::Helpers::toISODateWithMilliseconds(msg.time);
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
    else if (exactMatch("api/data"))
    {
        // api/data
        QJsonDocument document;
        QJsonObject json;
        QJsonArray samples;

        for (auto& sample : dataSamples)
        {
            QJsonObject messageObject;
            messageObject["time"] = common::Helpers::toISODateWithMilliseconds(sample.time);
            messageObject["field"] = sample.field;
            messageObject["qmc"] = sample.qmc;
            messageObject["state"] = sample.state;
            samples.append(messageObject);
        }

        json["samples"] = samples;
        document.setObject(json);
        auto jsonData = document.toJson(QJsonDocument::JsonFormat::Indented);

        mg_send_data(connection(), jsonData.data(), jsonData.size());
    }
    else
    {
        throw common::InvalidOperationException();
    }
}

QMutex* core::PosWebHandler::dataMutex()
{
    return &_dataMutex;
}
