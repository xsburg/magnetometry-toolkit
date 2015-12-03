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

namespace core
{
    struct MSeedSettings
    {
        QString location;
        QString network;
        QString station;
        QString fileName;
        int samplesInRecord;
    };

    struct RunnerConfig
    {
        int webServerPort;
        
        QString devicePortName;

        // mseed settings
        QString msRecordLocation;
        QString msRecordNetwork;
        QString msRecordStation;
        QString msFileName;
        int samplesCacheMaxSize;
    };
}
