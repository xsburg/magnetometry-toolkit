// ***********************************************************************
// <author>Stepan Burguchev</author>
// <copyright company="Comindware">
//   Copyright (c) Comindware 2010-2015. All rights reserved.
// </copyright>
// <summary>
//   EbDeviceLogger.h
// </summary>
// ***********************************************************************
#pragma once

#include "WebServer.h"
#include "RunnerActionHandler.h"
#include "RunnerData.h"
#include "MSeedRecord.h"
#include "MSeedWriter.h"

namespace core
{
    class Runner
    {
    public:
        SMART_PTR_T(Runner);
        Runner(RunnerConfig config);
        void run();
    private:
        void executeRunCommand(core::EbDevice::SharedPtr_t& device, RunnerCommand::SharedPtr_t& cmd, RunnerStatus::SharedPtr_t status);
        void executeStopCommand(core::EbDevice::SharedPtr_t& device, RunnerCommand::SharedPtr_t& cmd, RunnerStatus::SharedPtr_t status);
        void executeUpdateStatus(core::EbDevice::SharedPtr_t& device, RunnerCommand::SharedPtr_t& cmd, RunnerStatus::SharedPtr_t status);
        void executeSetTime(core::EbDevice::SharedPtr_t& device, RunnerCommand::SharedPtr_t& cmd, RunnerStatus::SharedPtr_t status);
        void executeSetRange(core::EbDevice::SharedPtr_t& device, RunnerCommand::SharedPtr_t& cmd, RunnerStatus::SharedPtr_t status);
        void executeSetStandBy(core::EbDevice::SharedPtr_t& device, RunnerCommand::SharedPtr_t& cmd, RunnerStatus::SharedPtr_t status);

        void log(Common::LogLevel level, const QString& message);
        void logInfo(const QString& message);
        void logDebug(const QString& message);
        void logError(const QString& message);

        IntegerMSeedRecord::SharedPtr_t createIntegerRecord(QString channelName, double samplingRateHz, QDateTime time);
        void flushSamplesCache(QVector<EbDevice::Sample>& samplesCache, MSeedWriter::SharedPtr_t writer, int samplingIntervalMs);

        WebServer::SharedPtr_t _webServer;
        RunnerActionHandler::SharedPtr_t _actionHandler;
        RunnerConfig _config;
        BufferedLogger::SharedPtr_t _logger;
    };
}
