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
    _webServer = std::make_shared<WebServer>();
    _webServer->port(config.webServerPort);
    _webServer->addActionHandler(_actionHandler);
}

void core::Runner::executeRunCommand(core::EbDevice::SharedPtr_t& device, RunnerCommand::SharedPtr_t& cmd, RunnerStatus::SharedPtr_t status)
{
    sLogger.Info(QString("Preparing command RUN..."));
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
    sLogger.Info(QString("Executing command RUN with { intervalMilliseconds: %1 (converted to %2) }...")
        .arg(samplingIntervalMs).arg(actualIntervalVal));
    device->sendAuto(actualIntervalVal);
    sLogger.Info(QString("Executed."));
}

void core::Runner::executeStopCommand(core::EbDevice::SharedPtr_t& device, RunnerCommand::SharedPtr_t& cmd, RunnerStatus::SharedPtr_t status)
{
    sLogger.Info(QString("Preparing command STOP..."));
    status->isRunning = false;
    status->updated = QDateTime::currentDateTimeUtc();
    sLogger.Info(QString("Executing command STOP..."));
    device->sendEnq();
    device->readEnq();
    sLogger.Info(QString("Executed."));
}

void core::Runner::executeUpdateStatus(core::EbDevice::SharedPtr_t& device, RunnerCommand::SharedPtr_t& cmd, RunnerStatus::SharedPtr_t status)
{
    sLogger.Info(QString("Executing command UPDATE-STATUS..."));

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

    sLogger.Info(QString("Executed."));
}

void core::Runner::executeSetTime(core::EbDevice::SharedPtr_t& device, RunnerCommand::SharedPtr_t& cmd, RunnerStatus::SharedPtr_t status)
{
    sLogger.Info(QString("Preparing command SET-TIME..."));
    auto timeCmd = std::static_pointer_cast<SetTimeRunnerCommand>(cmd);
    auto time = timeCmd->time();
    sLogger.Info(QString("Executing command SET-TIME..."));
    device->sendSetTime(time);
    device->readSetTime();
    device->sendGetTime();
    status->time = device->readGetTime();
    status->timeUpdated = QDateTime::currentDateTimeUtc();
    status->updated = status->timeUpdated;
    sLogger.Info(QString("Executed."));
}

void core::Runner::executeSetRange(core::EbDevice::SharedPtr_t& device, RunnerCommand::SharedPtr_t& cmd, RunnerStatus::SharedPtr_t status)
{
    sLogger.Info(QString("Preparing command SET-RANGE..."));
    auto rangeCmd = std::static_pointer_cast<SetRangeRunnerCommand>(cmd);
    auto center = rangeCmd->center();
    sLogger.Info(QString("Executing command SET-RANGE..."));
    device->sendSetRange(center);
    status->range = device->readSetRange();
    status->updated = QDateTime::currentDateTimeUtc();
    sLogger.Info(QString("Executed."));
}

void core::Runner::executeSetStandBy(core::EbDevice::SharedPtr_t& device, RunnerCommand::SharedPtr_t& cmd, RunnerStatus::SharedPtr_t status)
{
    sLogger.Info(QString("Preparing command SET-STAND-BY..."));
    auto standByCmd = std::static_pointer_cast<SetStandByRunnerCommand>(cmd);
    auto standBy = standByCmd->standBy();
    sLogger.Info(QString("Executing command SET-STAND-BY..."));
    device->sendStandBy(standBy);
    status->standBy = device->readStandBy();
    status->updated = QDateTime::currentDateTimeUtc();
    sLogger.Info(QString("Executed."));
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

void core::Runner::flushSamplesCache(QVector<EbDevice::Sample>& samplesCache, MSeedWriter::SharedPtr_t writer, int samplingIntervalMs)
{
    if (samplesCache.empty())
    {
        return;
    }
    sLogger.Debug(QString("Flushing samples cache (%1 samples)...").arg(samplesCache.size()));
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
    sLogger.Debug(QString("Done flushing."));
}

void core::Runner::run()
{
    // Running commands aggregator in background thread
    sLogger.Info(QString("Starting web server on port %1...").arg(_config.webServerPort));
    _webServer->runAsync();

    // Creating a device
    sLogger.Info(QString("Connecting to device on port %1...").arg(_config.devicePortName));
    auto device = std::make_shared<core::EbDevice>();
    device->connect(_config.devicePortName);
    if (!_config.skipDiagnostics)
    {
        sLogger.Info(QString("Running device diagnostics..."));
        device->runDiagnosticSequence();
        device->runTestAutoSequence();
        sLogger.Info(QString("Diagnostics done."));
    }
    else
    {
        sLogger.Info(QString("Skipping device diagnostics..."));
    }

    // Creating an mseed writer
    auto stream = std::make_shared<FileBinaryStream>(_config.msFileName, true);
    auto writer = std::make_shared<MSeedWriter>(stream);

    // We always do status update on start
    bool isRunning;
    bool isStopping = false;
    bool isFlushing = false;
    int samplingIntervalMs;
    {
        sLogger.Info(QString("Gathering device start-up config..."));
        QMutexLocker lock(_actionHandler->dataMutex());
        RunnerCommand::SharedPtr_t updateCmd = std::make_shared<UpdateStatusRunnerCommand>();
        executeUpdateStatus(device, updateCmd, _actionHandler->status());
        isRunning = _actionHandler->status()->isRunning;
        samplingIntervalMs = _actionHandler->status()->samplingIntervalMs;
        sLogger.Info(QString("Done gathering."));
    }

    QVector<EbDevice::Sample> samplesCache;
    // Main worker loop
    try
    {
        sLogger.Info(QString("Starting main logging loop..."));
        while (true)
        {
            if (isRunning)
            {
                if (!isStopping)
                {
                    // receive data sample and write it into mini-seed stream
                    const int acceptableDelay = 1000;
                    auto sample = device->readSample(samplingIntervalMs + acceptableDelay);
                    auto isValid = device->validateSample(sample);
                    sLogger.Info(QString("Received another sample: field: %1, time: %2.%3, state: 0x%4, qmc: %5, isValid: %6")
                        .arg(sample.field).arg(sample.time.toString(Qt::ISODate)).arg(sample.time.toMSecsSinceEpoch() % 1000)
                        .arg(sample.state, 2, 16).arg(sample.qmc).arg(isValid));

                    samplesCache.push_back(sample);

                    if (samplesCache.size() >= _config.samplesCacheMaxSize)
                    {
                        isFlushing = true;
                        flushSamplesCache(samplesCache, writer, samplingIntervalMs);
                        isFlushing = false;
                    }
                }
                else
                {
                    isFlushing = true;
                    flushSamplesCache(samplesCache, writer, samplingIntervalMs);
                    isFlushing = false;
                    isStopping = false;
                    isRunning = false;
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
                sLogger.Debug("Found a command...");
                auto cmd = _actionHandler->commands().dequeue();
                switch (cmd->type())
                {
                case Run:
                    executeRunCommand(device, cmd, _actionHandler->status());
                    isRunning = _actionHandler->status()->isRunning;
                    samplingIntervalMs = _actionHandler->status()->samplingIntervalMs;
                    break;
                case Stop:
                    if (isRunning)
                    {
                        executeStopCommand(device, cmd, _actionHandler->status());
                        isStopping = true;
                    }
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
                default:
                    throw Common::NotImplementedException();
                }
                _actionHandler->status()->commandQueueSize = _actionHandler->commands().size();
                sLogger.Debug("Done reading command.");
            }
        }
    }
    catch (...)
    {
        sLogger.Debug("Main runner loop has been broken by exception...");
        if (!isFlushing)
        {
            sLogger.Debug("Trying to flush samples cache...");
            flushSamplesCache(samplesCache, writer, samplingIntervalMs);
        }
        throw;
    }
}
