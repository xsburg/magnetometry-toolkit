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
#include "Common.h"
#include "MSeedSink.h"
#include <common/Thread.h>

namespace core
{
    class PosController
    {
    public:
        SMART_PTR_T(PosController);
        PosController(WebServer::SharedPtr_t webServer, MSeedSink::SharedPtr_t msSink, QString devicePortName, MSeedSettings msSettings);
        void run();
        void runAsync();
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
        void flushGatheredSamples();
        void handlePendingWebServerCommands();
        void handleNewDataSamples();

        WebServer::SharedPtr_t _webServer;
        MSeedSink::SharedPtr_t _msSink;
        PosWebHandler::SharedPtr_t _actionHandler;
        BufferedLogger::SharedPtr_t _webLogger;

        QString _devicePortName;

        MSeedSettings _msSettingsCache;
        int _samplingIntervalMsCache;

        // Runner loop components and data (erased on every loop cicle)
        PosDevice::SharedPtr_t _device;
        QVector<PosDevice::Sample> _gatheredSamples;
        bool _isRunning;
        bool _isFlushing;
        int _timeFixIntervalSeconds;
        common::Thread::SharedPtr_t _activeThread;
    };
}
