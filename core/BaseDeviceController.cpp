#include "BaseDeviceController.h"
#include "common/Logger.h"
#include "common/InvalidOperationException.h"
#include "common/Thread.h"

void core::BaseDeviceController::runAsync()
{
    if (_activeThread.get())
    {
        throw common::InvalidOperationException("The controller is already running.");
    }
    _activeThread = std::make_shared<common::Thread>([this]()
    {
        this->run();
    });
    _activeThread->start();
}

void core::BaseDeviceController::log(common::LogLevel level, const QString& message)
{
    if (_webLogger.get())
    {
        _webLogger->write(level, message);
    }
    sLogger.write(level, message);
}

void core::BaseDeviceController::logInfo(const QString& message)
{
    log(common::Info, message);
}

void core::BaseDeviceController::logDebug(const QString& message)
{
    log(common::Debug, message);
}

void core::BaseDeviceController::logError(const QString& message)
{
    log(common::Error, message);
}

core::IntegerMSeedRecord::SharedPtr_t core::BaseDeviceController::createIntegerRecord(QString channelName, double samplingRateHz, QDateTime time)
{
    auto record = std::make_shared<IntegerMSeedRecord>();
    record->channelName(channelName);
    record->location(_msSettingsCache.location);
    record->network(_msSettingsCache.network);
    record->station(_msSettingsCache.station);
    record->samplingRateHz(samplingRateHz);
    record->startTime(time);
    return record;
}
