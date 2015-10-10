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
#include "RunnerData.h"
#include "RunnerCommands.h"

namespace core
{
    class RunnerActionHandler : public core::WebServerActionHandler
    {
    public:
        SMART_PTR_T(RunnerActionHandler);

        RunnerActionHandler();

        QString name() const override
        {
            return "RunnerActionHandler";
        }

        bool match() override;
        void execute() override;
        QMutex* dataMutex();

        RunnerStatus::SharedPtr_t& status()
        {
            return _status;
        }

        BufferedLogger::SharedPtr_t& logger()
        {
            return _logger;
        }

        void setStatus(RunnerStatus::SharedPtr_t runnerStatus)
        {
            _status = runnerStatus;
        }

        QQueue<RunnerCommand::SharedPtr_t>& commands() { return _commands; }
    private:
        void addRunCommand(QJsonObject json);
        void addStopCommand(QJsonObject json);
        void addUpdateStatusCommand(QJsonObject json);
        void addSetTimeCommand(QJsonObject json);
        void addSetRangeCommand(QJsonObject json);
        void addSetStandByCommand(QJsonObject json);

        QQueue<RunnerCommand::SharedPtr_t> _commands;
        RunnerStatus::SharedPtr_t _status;
        BufferedLogger::SharedPtr_t _logger;
        QMutex _dataMutex;
    };
}
