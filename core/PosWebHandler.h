// ***********************************************************************
// <author>Stephan Burguchev</author>
// <copyright company="Stephan Burguchev">
//   Copyright (c) Stephan Burguchev 2012-2015. All rights reserved.
// </copyright>
// <summary>
//   RunnerActionHandler.h
// </summary>
// ***********************************************************************
#pragma once

#include "WebServer.h"
#include "Common.h"
#include "PosWebCommands.h"
#include "PosDevice.h"

namespace core
{
    struct PosStatus
    {
        SMART_PTR_T(PosStatus);

        PosStatus()
        {
            standBy = false;
            isRunning = false;
            samplingIntervalMs = 0;
            commandQueueSize = 0;
            timeFixIntervalSeconds = 0;
        }

        // Actually read from device
        PosDevice::RangeData range;
        QString enq;
        QString about;
        QDateTime time;

        // Other data
        QDateTime timeUpdated;
        QDateTime updated;
        bool standBy;
        bool isRunning;
        int samplingIntervalMs;
        int commandQueueSize;
        MSeedSettings mseedSettings;
        int timeFixIntervalSeconds;
    };

    class PosWebHandler : public core::WebServerActionHandler
    {
    public:
        SMART_PTR_T(PosWebHandler);

        PosWebHandler();

        QString name() const override
        {
            return "PosWebHandler";
        }

        bool match() override;
        void execute() override;
        QMutex* dataMutex();

        PosStatus::SharedPtr_t& status()
        {
            return _status;
        }

        BufferedLogger::SharedPtr_t& logger()
        {
            return _logger;
        }

        void setStatus(PosStatus::SharedPtr_t runnerStatus)
        {
            _status = runnerStatus;
        }

        QQueue<PosWebCommand::SharedPtr_t>& commands() { return _commands; }

        void addToDataBuffer(const PosDevice::Sample& sample);
    private:
        void addRunCommand(QJsonObject json);
        void addStopCommand(QJsonObject json);
        void addUpdateStatusCommand(QJsonObject json);
        void addSetTimeCommand(QJsonObject json);
        void addSetRangeCommand(QJsonObject json);
        void addSetStandByCommand(QJsonObject json);
        void addRunDiagnosticsCommand(QJsonObject json);
        void addRunModeAutoTestCommand(QJsonObject json);
        void addApplyMSeedSettingsCommand(QJsonObject json);

        static const int maxDataSamplesListSize = 100;

        QList<PosDevice::Sample> dataSamples;
        QQueue<PosWebCommand::SharedPtr_t> _commands;
        PosStatus::SharedPtr_t _status;
        BufferedLogger::SharedPtr_t _logger;
        QMutex _dataMutex;
    };
}
