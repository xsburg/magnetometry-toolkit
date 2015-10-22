#include "Runner.h"
#include <common/NotImplementedException.h>
#include "RunnerCommands.h"
#include <common/Logger.h>
#include "MSeedRecord.h"
#include "FileBinaryStream.h"
#include "MSeedWriter.h"

core::Runner::Runner(RunnerConfig config) : _config(config)
{
    _actionHandler = std::make_shared<RunnerActionHandler>();
    _logger = _actionHandler->logger();
    _webServer = std::make_shared<WebServer>();
    _webServer->port(config.webServerPort);
    _webServer->addActionHandler(_actionHandler);
}

void core::Runner::executeRunCommand(core::EbDevice::SharedPtr_t& device, RunnerCommand::SharedPtr_t& cmd, RunnerStatus::SharedPtr_t status)
{
    logInfo(QString("Preparing command RUN..."));
    auto runCmd = std::static_pointer_cast<RunRunnerCommand>(cmd);
    status->isRunning = true;
    status->updated = QDateTime::currentDateTimeUtc();
    int samplingIntervalMs = runCmd->intervalMilliseconds();
    int actualSamplingIntervalMs;
    int32_t actualIntervalVal;
    if (samplingIntervalMs > 1000)
    {
        actualIntervalVal = samplingIntervalMs / 1000;
        if (actualIntervalVal > 86400)
        {
            actualIntervalVal = 86400;
        }
        actualSamplingIntervalMs = actualIntervalVal * 1000;
    }
    else
    {
        // 5 4 3 2 Hz = 200ms, 250ms, 334ms, 500ms
        if (samplingIntervalMs <= 200)
        {
            actualIntervalVal = -5;
            actualSamplingIntervalMs = 200;
        }
        else if (samplingIntervalMs <= 250)
        {
            actualIntervalVal = -4;
            actualSamplingIntervalMs = 250;
        }
        else if (samplingIntervalMs <= 334)
        {
            actualIntervalVal = -3;
            actualSamplingIntervalMs = 334;
        }
        else if (samplingIntervalMs <= 500)
        {
            actualIntervalVal = -2;
            actualSamplingIntervalMs = 500;
        }
        else
        {
            actualIntervalVal = -1;
            actualSamplingIntervalMs = 1000;
        }
    }
    status->samplingIntervalMs = actualSamplingIntervalMs;
    logInfo(QString("Executing command RUN with { intervalMilliseconds: %1 (converted to %2) }...")
        .arg(samplingIntervalMs).arg(actualIntervalVal));
    device->sendAuto(actualIntervalVal);
    logInfo(QString("Executed."));
}

void core::Runner::executeStopCommand(core::EbDevice::SharedPtr_t& device, RunnerStatus::SharedPtr_t status)
{
    logInfo(QString("Preparing command STOP..."));
    status->isRunning = false;
    status->updated = QDateTime::currentDateTimeUtc();
    logInfo(QString("Executing command STOP..."));
    device->sendEnq();
    // We wait for the moment when there are no incoming messages. That means that the incoming data stream has stopped.
    int counter = 0;
    while (true)
    {
        auto messages = device->readAllResponseMessages();
        if (messages.size() == 0)
        {
            break;
        }
        counter++;
        if (counter > 10)
        {
            // We can't wait forever: restart runner if we stuck
            throw Common::Exception
        }
    }
    logInfo(QString("Executed."));
}

void core::Runner::executeUpdateStatus(core::EbDevice::SharedPtr_t& device, RunnerCommand::SharedPtr_t& cmd, RunnerStatus::SharedPtr_t status)
{
    logInfo(QString("Executing command UPDATE-STATUS..."));

    // enq
    device->sendEnq();
    status->enq = device->readEnq();
    // about
    device->sendAbout();
    status->about = device->readAbout();
    // range
    device->sendGetRange();
    status->range = device->readGetRange();
    // time
    device->sendGetTime();
    status->time = device->readGetTime();
    // timeUpdated
    status->timeUpdated = QDateTime::currentDateTimeUtc();
    // updated
    status->updated = status->timeUpdated;
    // commands size
    _actionHandler->status()->commandQueueSize = _actionHandler->commands().size();

    logInfo(QString("Executed."));
}

void core::Runner::executeSetTime(core::EbDevice::SharedPtr_t& device, RunnerCommand::SharedPtr_t& cmd, RunnerStatus::SharedPtr_t status)
{
    logInfo(QString("Preparing command SET-TIME..."));
    auto timeCmd = std::static_pointer_cast<SetTimeRunnerCommand>(cmd);
    auto time = timeCmd->time();
    logInfo(QString("Executing command SET-TIME..."));
    device->sendSetTime(time);
    device->readSetTime();
    device->sendGetTime();
    status->time = device->readGetTime();
    status->timeUpdated = QDateTime::currentDateTimeUtc();
    status->updated = status->timeUpdated;
    logInfo(QString("Executed."));
}

void core::Runner::executeSetRange(core::EbDevice::SharedPtr_t& device, RunnerCommand::SharedPtr_t& cmd, RunnerStatus::SharedPtr_t status)
{
    logInfo(QString("Preparing command SET-RANGE..."));
    auto rangeCmd = std::static_pointer_cast<SetRangeRunnerCommand>(cmd);
    auto center = rangeCmd->center();
    logInfo(QString("Executing command SET-RANGE..."));
    device->sendSetRange(center);
    status->range = device->readSetRange();
    status->updated = QDateTime::currentDateTimeUtc();
    logInfo(QString("Executed."));
}

void core::Runner::executeSetStandBy(core::EbDevice::SharedPtr_t& device, RunnerCommand::SharedPtr_t& cmd, RunnerStatus::SharedPtr_t status)
{
    logInfo(QString("Preparing command SET-STAND-BY..."));
    auto standByCmd = std::static_pointer_cast<SetStandByRunnerCommand>(cmd);
    auto standBy = standByCmd->standBy();
    logInfo(QString("Executing command SET-STAND-BY..."));
    device->sendStandBy(standBy);
    status->standBy = device->readStandBy();
    status->updated = QDateTime::currentDateTimeUtc();
    logInfo(QString("Executed."));
}

void core::Runner::executeDiagnostics(core::EbDevice::SharedPtr_t& device, RunnerCommand::SharedPtr_t& cmd, RunnerStatus::SharedPtr_t status)
{
    logInfo(QString("Executing command RUN-DIAGNOSTICS..."));
    device->runDiagnosticSequence();
    logInfo(QString("Executed."));
}

void core::Runner::executeAutoTest(core::EbDevice::SharedPtr_t& device, RunnerCommand::SharedPtr_t& cmd, RunnerStatus::SharedPtr_t status)
{
    logInfo(QString("Executing command AUTO-TEST..."));
    device->runTestAutoSequence();
    logInfo(QString("Executed."));
}

void core::Runner::executeApplyMSeedSettings(core::EbDevice::SharedPtr_t& device, RunnerCommand::SharedPtr_t& cmd, RunnerStatus::SharedPtr_t status)
{
    logInfo(QString("Executing command APPLY-MSEED-SETTINGS..."));
    auto applyMSeedSettingsCmd = std::static_pointer_cast<ApplyMSeedSettingsRunnerCommand>(cmd);
    auto newSettings = applyMSeedSettingsCmd->settings();
    logInfo(QString("Arguments: { fileName: '%1', network: '%2', station: '%3', location: '%4', samplesInRecord: %5 }")
        .arg(newSettings.fileName)
        .arg(newSettings.network)
        .arg(newSettings.station)
        .arg(newSettings.location)
        .arg(newSettings.samplesInRecord));
    _actionHandler->status()->mseedSettings.fileName = newSettings.fileName;
    _actionHandler->status()->mseedSettings.network = newSettings.network;
    _actionHandler->status()->mseedSettings.station = newSettings.station;
    _actionHandler->status()->mseedSettings.location = newSettings.location;
    _actionHandler->status()->mseedSettings.samplesInRecord = newSettings.samplesInRecord;
    logInfo(QString("Executed."));
}

core::IntegerMSeedRecord::SharedPtr_t core::Runner::createIntegerRecord(QString channelName, double samplingRateHz, QDateTime time)
{
    auto record = std::make_shared<IntegerMSeedRecord>();
    record->channelName(channelName);
    record->location(_config.msRecordLocation);
    record->network(_config.msRecordNetwork);
    record->station(_config.msRecordStation);
    record->samplingRateHz(samplingRateHz);
    record->startTime(time);
    return record;
}

void core::Runner::flushSamplesCache(QVector<EbDevice::Sample>& samplesCache, MSeedWriter::SharedPtr_t writer, int samplingIntervalMs, bool* isFlushing)
{
    if (samplesCache.empty())
    {
        return;
    }
    *isFlushing = true;
    logDebug(QString("Flushing samples cache (%1 samples)...").arg(samplesCache.size()));
    double samplingRateHz = 1000.0 / samplingIntervalMs;

    for (auto& sample : samplesCache)
    {
        auto fieldRecord = createIntegerRecord("FLD", samplingRateHz, sample.time);
        fieldRecord->data().push_back(sample.field);
        writer->write(fieldRecord);

        auto qualityRecord = createIntegerRecord("QMC", samplingRateHz, sample.time);
        qualityRecord->data().push_back(sample.qmc);
        writer->write(qualityRecord);

        auto stateRecord = createIntegerRecord("STT", samplingRateHz, sample.time);
        stateRecord->data().push_back(sample.state);
        writer->write(stateRecord);
    }

    writer->flush();
    samplesCache.clear();
    logDebug(QString("Done flushing."));
    *isFlushing = false;
}

void core::Runner::run()
{
    // Running commands aggregator in background thread
    sLogger.info(QString("Starting web server on port %1...").arg(_config.webServerPort));
    _webServer->runAsync();

    // Creating a device
    sLogger.info(QString("Connecting to device on port %1...").arg(_config.devicePortName));
    auto device = std::make_shared<EbDevice>(_logger);
    device->connect(_config.devicePortName);
    if (!_config.skipDiagnostics)
    {
        sLogger.info(QString("Running device diagnostics..."));
        device->runDiagnosticSequence();
        device->runTestAutoSequence();
        sLogger.info(QString("Diagnostics done."));
    }
    else
    {
        sLogger.info(QString("Skipping device diagnostics..."));
    }

    // Creating an mseed writer
    auto stream = std::make_shared<FileBinaryStream>(_config.msFileName, true);
    auto writer = std::make_shared<MSeedWriter>(stream);

    // We always do status update on start
    bool isRunning;
    bool isFlushing = false;
    int samplingIntervalMs;
    {
        sLogger.info(QString("Gathering device start-up config..."));
        QMutexLocker lock(_actionHandler->dataMutex());
        RunnerCommand::SharedPtr_t updateCmd = std::make_shared<UpdateStatusRunnerCommand>();
        executeUpdateStatus(device, updateCmd, _actionHandler->status());
        isRunning = _actionHandler->status()->isRunning;
        samplingIntervalMs = _actionHandler->status()->samplingIntervalMs;
        _actionHandler->status()->mseedSettings.fileName = _config.msFileName;
        _actionHandler->status()->mseedSettings.network = _config.msRecordNetwork;
        _actionHandler->status()->mseedSettings.station = _config.msRecordStation;
        _actionHandler->status()->mseedSettings.location = _config.msRecordLocation;
        _actionHandler->status()->mseedSettings.samplesInRecord = _config.samplesCacheMaxSize;
        sLogger.info(QString("Done gathering."));
    }

    QVector<EbDevice::Sample> samplesCache;
    // Main worker loop
    try
    {
        sLogger.info(QString("Starting main logging loop..."));
        while (true)
        {
            if (isRunning)
            {
                // receive data sample and write it into mini-seed stream
                const int acceptableDelay = 1000;
                auto sample = device->readSample(samplingIntervalMs + acceptableDelay);
                auto isValid = device->validateSample(sample);
                sLogger.info(QString("Received another sample: field: %1, time: %2.%3, state: 0x%4, qmc: %5, isValid: %6")
                    .arg(sample.field).arg(sample.time.toString(Qt::ISODate)).arg(sample.time.toMSecsSinceEpoch() % 1000)
                    .arg(sample.state, 2, 16).arg(sample.qmc).arg(isValid));

                samplesCache.push_back(sample);
                {
                    QMutexLocker lock(_actionHandler->dataMutex());
                    _actionHandler->addToDataBuffer(sample);
                }

                if (samplesCache.size() >= _config.samplesCacheMaxSize)
                {
                    flushSamplesCache(samplesCache, writer, samplingIntervalMs, &isFlushing);
                }
            }
            else
            {
                // sleep 1 second as a fallback for data logging
                QThread::sleep(1);
            }

            // run commands
            QMutexLocker lock(_actionHandler->dataMutex());
            while (!_actionHandler->commands().empty())
            {
                sLogger.debug("Found a command...");

                if (isRunning)
                {
                    flushSamplesCache(samplesCache, writer, samplingIntervalMs, &isFlushing);
                    executeStopCommand(device, _actionHandler->status());
                    isRunning = false;
                }

                auto cmd = _actionHandler->commands().dequeue();
                switch (cmd->type())
                {
                case Run:
                    executeRunCommand(device, cmd, _actionHandler->status());
                    isRunning = _actionHandler->status()->isRunning;
                    samplingIntervalMs = _actionHandler->status()->samplingIntervalMs;
                    break;
                case Stop:
                    // Any command make a stop first
                    break;
                case UpdateStatus:
                    executeUpdateStatus(device, cmd, _actionHandler->status());
                    break;
                case SetTime:
                    executeSetTime(device, cmd, _actionHandler->status());
                    break;
                case SetRange:
                    executeSetRange(device, cmd, _actionHandler->status());
                    break;
                case SetStandBy:
                    executeSetStandBy(device, cmd, _actionHandler->status());
                    break;
                case RunDiagnostics:
                    executeDiagnostics(device, cmd, _actionHandler->status());
                    executeUpdateStatus(device, cmd, _actionHandler->status());
                    break;
                case RunModeAutoTest:
                    executeAutoTest(device, cmd, _actionHandler->status());
                    break;
                case ApplyMSeedSettings:
                    executeApplyMSeedSettings(device, cmd, _actionHandler->status());
                    break;
                default:
                    throw Common::NotImplementedException();
                }
                _actionHandler->status()->commandQueueSize = _actionHandler->commands().size();
                sLogger.debug("Done reading command.");
            }
        }
    }
    catch (...)
    {
        sLogger.debug("Main runner loop has been broken by exception...");
        if (!isFlushing)
        {
            sLogger.debug("Trying to flush samples cache...");
            flushSamplesCache(samplesCache, writer, samplingIntervalMs, &isFlushing);
        }
        throw;
    }
}

void core::Runner::log(Common::LogLevel level, const QString& message)
{
    if (_logger.get())
    {
        _logger->write(level, message);
    }
    sLogger.write(level, message);
}

void core::Runner::logInfo(const QString& message)
{
    log(Common::Info, message);
}

void core::Runner::logDebug(const QString& message)
{
    log(Common::Debug, message);
}

void core::Runner::logError(const QString& message)
{
    log(Common::Error, message);
}
