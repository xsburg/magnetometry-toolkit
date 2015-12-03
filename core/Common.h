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

#include <QtCore/QString>

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
}
