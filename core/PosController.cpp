#include "PosController.h"
#include <common/NotImplementedException.h>
#include <common/Logger.h>
#include "MSeedRecord.h"
#include "FileBinaryStream.h"
#include "MSeedWriter.h"

core::PosController::PosController(WebServer::SharedPtr_t webServer, QString devicePortName)
    : _devicePortName(devicePortName), _isRunning(false), _isFlushing(false), _samplingIntervalMs(0), _timeFixIntervalSeconds(0)
{
    _actionHandler = std::make_shared<PosWebHandler>();
    _webLogger = _actionHandler->logger();
    _webServer->addActionHandler(_actionHandler);
}

void core::PosController::executeRunCommand(QMutexLocker& dataLock, core::PosDevice::SharedPtr_t& device, int samplingIntervalMs, int timeFixIntervalSeconds, PosStatus::SharedPtr_t status)
{
    logInfo(QString("Preparing command RUN..."));
    status->isRunning = true;
    status->updated = QDateTime::currentDateTimeUtc();
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
    status->timeFixIntervalSeconds = timeFixIntervalSeconds;
    logInfo(QString("Executing command RUN with { intervalMilliseconds: %1 (converted to %2) }...")
        .arg(samplingIntervalMs).arg(actualIntervalVal));
    dataLock.unlock();
    device->sendAuto(actualIntervalVal);
    dataLock.relock();
    logInfo(QString("Executed."));
    _isRunning = _actionHandler->status()->isRunning;
    _samplingIntervalMs = _actionHandler->status()->samplingIntervalMs;
    _timeFixIntervalSeconds = timeFixIntervalSeconds;
}

void core::PosController::executeStopCommand(QMutexLocker& dataLock, core::PosDevice::SharedPtr_t& device, PosStatus::SharedPtr_t status)
{
    logInfo(QString("Executing command STOP..."));
    dataLock.unlock();
    device->sendEnq();
    // We wait for the moment when there are no incoming messages. That means that the incoming data stream has stopped.
    device->waitForInputSilence();
    dataLock.relock();
    status->isRunning = false;
    status->updated = QDateTime::currentDateTimeUtc();
    logInfo(QString("Executed."));
}

void core::PosController::executeUpdateStatus(QMutexLocker& dataLock, core::PosDevice::SharedPtr_t& device, PosStatus::SharedPtr_t status)
{
    logInfo(QString("Executing command UPDATE-STATUS..."));

    dataLock.unlock();
    // enq
    device->sendEnq();
    auto newEnq = device->readEnq();
    // about
    device->sendAbout();
    auto newAbout = device->readAbout();
    // range
    device->sendGetRange();
    auto newRange = device->readGetRange();
    // time
    device->sendGetTime();
    auto newTime = device->readGetTime();
    dataLock.relock();

    status->enq = newEnq;
    status->about = newAbout;
    status->range = newRange;
    status->time = newTime;

    // timeUpdated
    status->timeUpdated = QDateTime::currentDateTimeUtc();
    // updated
    status->updated = status->timeUpdated;
    // commands size
    _actionHandler->status()->commandQueueSize = _actionHandler->commands().size();

    logInfo(QString("Executed."));
}

void core::PosController::executeSetTime(QMutexLocker& dataLock, core::PosDevice::SharedPtr_t& device, QDateTime time, PosStatus::SharedPtr_t status)
{
    logInfo(QString("Executing command SET-TIME..."));
    dataLock.unlock();
    device->sendSetTime(time);
    device->readSetTime();
    device->sendGetTime();
    auto newTime = device->readGetTime();
    dataLock.relock();
    status->time = newTime;
    status->timeUpdated = QDateTime::currentDateTimeUtc();
    status->updated = status->timeUpdated;
    logInfo(QString("Executed."));
}

void core::PosController::executeSetRange(QMutexLocker& dataLock, core::PosDevice::SharedPtr_t& device, uint32_t center, PosStatus::SharedPtr_t status)
{
    logInfo(QString("Executing command SET-RANGE..."));
    dataLock.unlock();
    device->sendSetRange(center);
    auto newRange = device->readSetRange();
    dataLock.relock();
    status->range = newRange;
    status->updated = QDateTime::currentDateTimeUtc();
    logInfo(QString("Executed."));
}

void core::PosController::executeSetStandBy(QMutexLocker& dataLock, core::PosDevice::SharedPtr_t& device, bool standBy, PosStatus::SharedPtr_t status)
{
    logInfo(QString("Executing command SET-STAND-BY..."));
    dataLock.unlock();
    device->sendStandBy(standBy);
    auto newStandBy = device->readStandBy();
    dataLock.relock();
    status->standBy = newStandBy;
    status->updated = QDateTime::currentDateTimeUtc();
    logInfo(QString("Executed."));
}

void core::PosController::executeDiagnostics(QMutexLocker& dataLock, core::PosDevice::SharedPtr_t& device, PosStatus::SharedPtr_t status)
{
    logInfo(QString("Executing command RUN-DIAGNOSTICS..."));
    dataLock.unlock();
    device->runDiagnosticSequence();
    dataLock.relock();
    logInfo(QString("Executed."));
}

void core::PosController::executeAutoTest(QMutexLocker& dataLock, core::PosDevice::SharedPtr_t& device, PosStatus::SharedPtr_t status)
{
    logInfo(QString("Executing command AUTO-TEST..."));
    dataLock.unlock();
    device->runTestAutoSequence();
    dataLock.relock();
    logInfo(QString("Executed."));
}

void core::PosController::executeApplyMSeedSettings(QMutexLocker& dataLock, core::PosDevice::SharedPtr_t& device, core::MSeedSettings newSettings, PosStatus::SharedPtr_t status)
{
    logInfo(QString("Executing command APPLY-MSEED-SETTINGS..."));
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

core::IntegerMSeedRecord::SharedPtr_t core::PosController::createIntegerRecord(QString channelName, double samplingRateHz, QDateTime time)
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

void core::PosController::flushSamplesCache()
{
    if (_samplesCache.empty() || _isFlushing)
    {
        return;
    }
    _isFlushing = true;
    logDebug(QString("Flushing samples cache (%1 samples)...").arg(_samplesCache.size()));
    double samplingRateHz = 1000.0 / _samplingIntervalMs;

    auto recordTime = _samplesCache.first().time;

    // field
    auto fieldRecord = createIntegerRecord("FLD", samplingRateHz, recordTime);
    for (auto& sample : _samplesCache)
    {
        fieldRecord->data().push_back(sample.field);
    }
    _writer->write(fieldRecord);
    // qmc
    auto qualityRecord = createIntegerRecord("QMC", samplingRateHz, recordTime);
    for (auto& sample : _samplesCache)
    {
        qualityRecord->data().push_back(sample.qmc);
    }
    _writer->write(qualityRecord);
    // state
    auto stateRecord = createIntegerRecord("STT", samplingRateHz, recordTime);
    for (auto& sample : _samplesCache)
    {
        stateRecord->data().push_back(sample.state);
    }
    _writer->write(stateRecord);

    _writer->flush();
    _samplesCache.clear();
    logDebug(QString("Done flushing."));
    _isFlushing = false;
}

void core::PosController::handlePendingWebServerCommands()
{
    QMutexLocker lock(_actionHandler->dataMutex());
    while (!_actionHandler->commands().empty())
    {
        sLogger.debug("Found a command...");

        if (_isRunning)
        {
            flushSamplesCache();
            executeStopCommand(lock, _device, _actionHandler->status());
            _isRunning = false;
        }

        auto cmd = _actionHandler->commands().dequeue();
        switch (cmd->type())
        {
        case Run:
            {
                if (!_isRunning)
                {
                    auto runCmd = std::static_pointer_cast<PosRunWebCommand>(cmd);
                    int samplingIntervalMs = runCmd->intervalMilliseconds();
                    int timeFixIntervalSeconds = runCmd->timeFixIntervalSeconds();
                    executeRunCommand(lock, _device, samplingIntervalMs, timeFixIntervalSeconds, _actionHandler->status());
                }
            }
            break;
        case Stop:
            // Any command make a stop first
            break;
        case UpdateStatus:
            executeUpdateStatus(lock, _device, _actionHandler->status());
            break;
        case SetTime:
            {
                auto timeCmd = std::static_pointer_cast<PosSetTimeWebCommand>(cmd);
                QDateTime time = timeCmd->time();
                executeSetTime(lock, _device, time, _actionHandler->status());
            }
            break;
        case SetRange:
            {
                auto rangeCmd = std::static_pointer_cast<PosSetRangeWebCommand>(cmd);
                uint32_t center = rangeCmd->center();
                executeSetRange(lock, _device, center, _actionHandler->status());
            }
            break;
        case SetStandBy:
            {
                auto standByCmd = std::static_pointer_cast<PosSetStandByWebCommand>(cmd);
                bool standBy = standByCmd->standBy();
                executeSetStandBy(lock, _device, standBy, _actionHandler->status());
            }
            break;
        case RunDiagnostics:
            executeDiagnostics(lock, _device, _actionHandler->status());
            executeUpdateStatus(lock, _device, _actionHandler->status());
            break;
        case RunModeAutoTest:
            executeAutoTest(lock, _device, _actionHandler->status());
            break;
        case ApplyMSeedSettings:
            {
                auto applyMSeedSettingsCmd = std::static_pointer_cast<PosApplyMSeedSettingsWebCommand>(cmd);
                auto newSettings = applyMSeedSettingsCmd->settings();
                executeApplyMSeedSettings(lock, _device, newSettings, _actionHandler->status());
            }
            break;
        default:
            throw common::NotImplementedException();
        }
        _actionHandler->status()->commandQueueSize = _actionHandler->commands().size();
        sLogger.debug("Done reading command.");
    }
}

void core::PosController::handleNewDataSamples()
{
    // receive data sample and write it into mini-seed stream
    const int acceptableDelay = 1000;
    auto samples = _device->readAllSamples(_samplingIntervalMs + acceptableDelay);
    for (auto& sample : samples)
    {
        auto isValid = _device->validateSample(sample);
        sLogger.info(QString("Received another sample: field: %1, time: %2.%3, state: 0x%4, qmc: %5, isValid: %6")
            .arg(sample.field).arg(sample.time.toString(Qt::ISODate)).arg(sample.time.toMSecsSinceEpoch() % 1000)
            .arg(sample.state, 2, 16).arg(sample.qmc).arg(isValid));

        _samplesCache.push_back(sample);
        {
            QMutexLocker lock(_actionHandler->dataMutex());
            _actionHandler->addToDataBuffer(sample);
        }

        if (_samplesCache.size() >= _config.samplesCacheMaxSize)
        {
            flushSamplesCache();
        }
    }
}

void core::PosController::run()
{
    // Error-restart loop
    while (true)
    {
        // Creating a device
        sLogger.info(QString("Connecting to device on port %1...").arg(_devicePortName));
        _device = std::make_shared<PosDevice>(_webLogger);
        _device->connect(_devicePortName);
        sLogger.info(QString("Connected."));

        // Creating an mseed writer
        auto stream = std::make_shared<FileBinaryStream>(_config.msFileName, true);
        _writer = std::make_shared<MSeedWriter>(stream);
        _writer->verbose(MSeedPackVerbose::None);

        // We always do status update on start
        _isRunning = false;
        _isFlushing = false;
        _samplingIntervalMs = 0;
        _timeFixIntervalSeconds = 0;
        {
            sLogger.info(QString("Gathering device start-up config..."));
            QMutexLocker lock(_actionHandler->dataMutex());
            PosWebCommand::SharedPtr_t updateCmd = std::make_shared<PosUpdateStatusWebCommand>();
            executeUpdateStatus(lock, _device, _actionHandler->status());
            _isRunning = _actionHandler->status()->isRunning;
            _samplingIntervalMs = _actionHandler->status()->samplingIntervalMs;
            _timeFixIntervalSeconds = _actionHandler->status()->timeFixIntervalSeconds;
            _actionHandler->status()->mseedSettings.fileName = _config.msFileName;
            _actionHandler->status()->mseedSettings.network = _config.msRecordNetwork;
            _actionHandler->status()->mseedSettings.station = _config.msRecordStation;
            _actionHandler->status()->mseedSettings.location = _config.msRecordLocation;
            _actionHandler->status()->mseedSettings.samplesInRecord = _config.samplesCacheMaxSize;
            sLogger.info(QString("Done gathering."));
        }

        // Main worker loop
        _samplesCache = QVector<PosDevice::Sample>();
        qint64 lastTimeFixEpoch = QDateTime::currentMSecsSinceEpoch();
        try
        {
            sLogger.info(QString("Starting main logging loop..."));
            while (true)
            {
                if (_isRunning)
                {
                    // Performing device time fix if required
                    qint64 nowEpoch = QDateTime::currentMSecsSinceEpoch();
                    if (_timeFixIntervalSeconds > 0 && nowEpoch - lastTimeFixEpoch > _timeFixIntervalSeconds * 1000)
                    {
                        sLogger.info("Performing device time correction...");
                        QMutexLocker lock(_actionHandler->dataMutex());
                        executeStopCommand(lock, _device, _actionHandler->status());
                        _actionHandler->status()->isRunning = true; // simulating that we are still running
                        executeSetTime(lock, _device, QDateTime::currentDateTimeUtc(), _actionHandler->status());
                        executeRunCommand(lock, _device, _samplingIntervalMs, _timeFixIntervalSeconds, _actionHandler->status());
                        lastTimeFixEpoch = QDateTime::currentMSecsSinceEpoch();
                    }

                    handleNewDataSamples();
                }
                else
                {
                    // sleep 1 second as a fallback for data logging
                    QThread::sleep(1);
                }

                handlePendingWebServerCommands();
            }
        }
        catch (PosDeviceException& ex)
        {
            sLogger.error("Main runner loop has been broken by PosDeviceException.");
            sLogger.error(QString("The what() message: %1.").arg(ex.what()));
            flushSamplesCache();
        }
        catch (common::Exception& ex)
        {
            sLogger.error("Main runner loop has been broken by common::Exception.");
            sLogger.error(QString("The what() message: %1.").arg(ex.what()));
            flushSamplesCache();
        }
        catch (std::exception& ex)
        {
            sLogger.error("Main runner loop has been broken by std::exception.");
            sLogger.error(QString("The what() message: %1.").arg(ex.what()));
            sLogger.error(QString("The samples cache won't be flushed because we don't know the exact reason of the failure."));
        }
        catch (...)
        {
            sLogger.error("Main runner loop has been broken by unknown exception.");
            sLogger.error(QString("The samples cache won't be flushed because we don't know the exact reason of the failure."));
        }
    }
}

void core::PosController::log(common::LogLevel level, const QString& message)
{
    if (_webLogger.get())
    {
        _webLogger->write(level, message);
    }
    sLogger.write(level, message);
}

void core::PosController::logInfo(const QString& message)
{
    log(common::Info, message);
}

void core::PosController::logDebug(const QString& message)
{
    log(common::Debug, message);
}

void core::PosController::logError(const QString& message)
{
    log(common::Error, message);
}
