// ***********************************************************************
// <author>Stepan Burguchev</author>
// <copyright company="Comindware">
//   Copyright (c) Comindware 2010-2015. All rights reserved.
// </copyright>
// <summary>
//   PosDeviceLogger.h
// </summary>
// ***********************************************************************
#pragma once

#include "WebServer.h"
#include "PosWebHandler.h"
#include "MSeedRecord.h"
#include "MSeedWriter.h"
#include "Common.h"

namespace core
{
    class PosController
    {
    public:
        SMART_PTR_T(PosController);
        PosController(WebServer::SharedPtr_t webServer, QString devicePortName);
        void run();
    private:
        void executeRunCommand(QMutexLocker& dataLock, core::PosDevice::SharedPtr_t& device, int samplingIntervalMs, int timeFixIntervalSeconds, PosStatus::SharedPtr_t status);
        void executeStopCommand(QMutexLocker& dataLock, core::PosDevice::SharedPtr_t& device, PosStatus::SharedPtr_t status);
        void executeUpdateStatus(QMutexLocker& dataLock, core::PosDevice::SharedPtr_t& device, PosStatus::SharedPtr_t status);
        void executeSetTime(QMutexLocker& dataLock, core::PosDevice::SharedPtr_t& device, QDateTime time, PosStatus::SharedPtr_t status);
        void executeSetRange(QMutexLocker& dataLock, core::PosDevice::SharedPtr_t& device, uint32_t center, PosStatus::SharedPtr_t status);
        void executeSetStandBy(QMutexLocker& dataLock, core::PosDevice::SharedPtr_t& device, bool standBy, PosStatus::SharedPtr_t status);
        void executeDiagnostics(QMutexLocker& dataLock, core::PosDevice::SharedPtr_t& device, PosStatus::SharedPtr_t status);
        void executeAutoTest(QMutexLocker& dataLock, core::PosDevice::SharedPtr_t& device, PosStatus::SharedPtr_t status);
        void executeApplyMSeedSettings(QMutexLocker& dataLock, core::PosDevice::SharedPtr_t& device, core::MSeedSettings newSettings, PosStatus::SharedPtr_t status);

        void log(common::LogLevel level, const QString& message);
        void logInfo(const QString& message);
        void logDebug(const QString& message);
        void logError(const QString& message);

        IntegerMSeedRecord::SharedPtr_t createIntegerRecord(QString channelName, double samplingRateHz, QDateTime time);
        void flushSamplesCache();
        void handlePendingWebServerCommands();
        void handleNewDataSamples();

        WebServer::SharedPtr_t _webServer;
        PosWebHandler::SharedPtr_t _actionHandler;
        BufferedLogger::SharedPtr_t _webLogger;

        // Runner loop components and data
        PosDevice::SharedPtr_t _device;
        MSeedWriter::SharedPtr_t _writer;
        QVector<PosDevice::Sample> _samplesCache;
        bool _isRunning;
        bool _isFlushing;
        int _samplingIntervalMs;
        int _timeFixIntervalSeconds;

        QString _devicePortName;
    };
}
