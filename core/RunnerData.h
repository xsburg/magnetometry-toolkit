// ***********************************************************************
// <author>Stephan Burguchev</author>
// <copyright company="Stephan Burguchev">
//   Copyright (c) Stephan Burguchev 2012-2015. All rights reserved.
// </copyright>
// <summary>
//   RunnerData.h
// </summary>
// ***********************************************************************

#pragma once

#include "EbDevice.h"

namespace core
{
    struct EbDeviceStatus
    {
        SMART_PTR_T(EbDeviceStatus);

        EbDevice::RangeData range;
        QString enq;
        QString about;
        QDateTime time;
    };

    struct MSeedSettings
    {
        QString location;
        QString network;
        QString station;
        QString fileName;
        int samplesInRecord;
    };

    struct RunnerStatus : EbDeviceStatus
    {
        SMART_PTR_T(RunnerStatus);

        RunnerStatus()
        {
            standBy = false;
            isRunning = false;
            samplingIntervalMs = 0;
            commandQueueSize = 0;
            timeFixIntervalSeconds = 0;
        }

        QDateTime timeUpdated;
        QDateTime updated;
        bool standBy;
        bool isRunning;
        int samplingIntervalMs;
        int commandQueueSize;
        MSeedSettings mseedSettings;
        int timeFixIntervalSeconds;
    };

    struct RunnerConfig
    {
        int webServerPort;
        QString devicePortName;
        QString msRecordLocation;
        QString msRecordNetwork;
        QString msRecordStation;
        QString msFileName;
        int samplesCacheMaxSize;
        bool skipDiagnostics;
    };
}
