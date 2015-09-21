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

    struct RunnerStatus : EbDeviceStatus
    {
        SMART_PTR_T(RunnerStatus);
        QDateTime updated;
        bool standBy;
    };

    struct RunnerConfig
    {
        int webServerPort;
        QString devicePortName;
    };
}
