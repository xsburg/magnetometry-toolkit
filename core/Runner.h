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

namespace core
{
    class Runner
    {
    public:
        SMART_PTR_T(Runner);
        Runner(RunnerConfig config);
        void executeRunCommand(core::EbDevice::SharedPtr_t& device, RunnerCommand::SharedPtr_t& cmd, bool* isRunning);
        void executeStopCommand(core::EbDevice::SharedPtr_t& device, RunnerCommand::SharedPtr_t& cmd, bool* isRunning);
        void run();
    private:
        WebServer::SharedPtr_t _webServer;
        RunnerActionHandler::SharedPtr_t _actionHandler;
        RunnerConfig _config;
    };
}
