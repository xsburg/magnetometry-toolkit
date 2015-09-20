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

namespace core
{
    class RunnerActionHandler : public core::WebServerActionHandler
    {
    public:
        QString name() const override
        {
            return "RunnerActionHandler";
        }

        bool match() override;
        void executeRunCommand(QJsonObject json);
        void executeStopCommand(QJsonObject json);
        void executeUpdateStatusCommand(QJsonObject json);
        void executeSetTimeCommand(QJsonObject json);
        void executeSetRangeCommand(QJsonObject json);
        void executeSetStandByCommand(QJsonObject json);
        void execute() override;
    };

    struct RunnerConfig
    {
        int webServerPort;
    };

    class Runner
    {
    public:
        Runner(RunnerConfig config);

        void run();
    private:
        WebServer::SharedPtr_t _webServer;
        RunnerActionHandler::SharedPtr_t _actionHandler;
    };
}
