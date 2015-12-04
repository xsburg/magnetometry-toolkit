// ***********************************************************************
// <author>Stepan Burguchev</author>
// <copyright company="Comindware">
//   Copyright (c) Comindware 2010-2015. All rights reserved.
// </copyright>
// <summary>
//   BaseDeviceController.h
// </summary>
// ***********************************************************************
#pragma once

#include <common/Thread.h>
#include "BufferedLogger.h"
#include "MSeedRecord.h"
#include "Common.h"

class QString;

namespace common{
    enum LogLevel;
}

namespace core
{
    class BaseDeviceController
    {
    protected:
        ~BaseDeviceController()
        {
        }

        void log(common::LogLevel level, const QString& message);
        void logInfo(const QString& message);
        void logDebug(const QString& message);
        void logError(const QString& message);

        IntegerMSeedRecord::SharedPtr_t createIntegerRecord(QString channelName, double samplingRateHz, QDateTime time);

        BufferedLogger::SharedPtr_t _webLogger;
        MSeedSettings _msSettingsCache;
    public:
        void runAsync();
        virtual void run() = 0;

    private:
        std::shared_ptr<common::Thread> _activeThread;
    };
}
