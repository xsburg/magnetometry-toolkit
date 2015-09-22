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
        void executeRunCommand(core::EbDevice::SharedPtr_t& device, RunnerCommand::SharedPtr_t& cmd, RunnerStatus::SharedPtr_t status);
        void executeStopCommand(core::EbDevice::SharedPtr_t& device, RunnerCommand::SharedPtr_t& cmd, RunnerStatus::SharedPtr_t status);
        void executeUpdateStatus(core::EbDevice::SharedPtr_t& device, RunnerCommand::SharedPtr_t& cmd, RunnerStatus::SharedPtr_t status);
        void executeSetTime(core::EbDevice::SharedPtr_t& device, RunnerCommand::SharedPtr_t& cmd, RunnerStatus::SharedPtr_t status);
        void executeSetRange(core::EbDevice::SharedPtr_t& device, RunnerCommand::SharedPtr_t& cmd, RunnerStatus::SharedPtr_t status);
        void executeSetStandBy(core::EbDevice::SharedPtr_t& device, RunnerCommand::SharedPtr_t& cmd, RunnerStatus::SharedPtr_t status);
        void run();
    private:
        WebServer::SharedPtr_t _webServer;
        RunnerActionHandler::SharedPtr_t _actionHandler;
        RunnerConfig _config;
    };
}
