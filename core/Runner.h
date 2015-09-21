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
    enum RunnerCommandType
    {
        Run,
        Stop,
        UpdateStatus,
        SetTime,
        SetRange,
        SetStandBy
    };

    class RunnerCommand
    {
    public:
        SMART_PTR_T(RunnerCommand);
        
        explicit RunnerCommand()
        {
        }

        virtual ~RunnerCommand()
        {
        }

        virtual RunnerCommandType type() const = 0;
    };

    class RunRunnerCommand : RunnerCommand
    {
    public:
        RunnerCommandType type() const override
        {
            return RunnerCommandType::Run;
        }
    };

    class StopRunnerCommand : RunnerCommand
    {
    public:
        RunnerCommandType type() const override
        {
            return RunnerCommandType::Stop;
        }
    };

    class UpdateStatusRunnerCommand : RunnerCommand
    {
    public:
        RunnerCommandType type() const override
        {
            return RunnerCommandType::UpdateStatus;
        }
    };

    class SetTimeRunnerCommand : RunnerCommand
    {
        QDateTime _time;
    public:
        explicit SetTimeRunnerCommand(const QDateTime& qDateTime)
            : _time(qDateTime)
        {
        }

        QDateTime time() const
        {
            return _time;
        }

        RunnerCommandType type() const override
        {
            return RunnerCommandType::SetTime;
        }
    };

    class SetRangeRunnerCommand : RunnerCommand
    {
        uint32_t _center;
    public:
        explicit SetRangeRunnerCommand(uint32_t uint32_t)
            : _center(uint32_t)
        {
        }

        RunnerCommandType type() const override
        {
            return RunnerCommandType::SetRange;
        }
        
        uint32_t center() const
        {
            return _center;
        }
    };

    class SetStandByRunnerCommand : RunnerCommand
    {
        bool _standBy;
    public:
        explicit SetStandByRunnerCommand(bool standBy)
            : _standBy(standBy)
        {
        }

        RunnerCommandType type() const override
        {
            return RunnerCommandType::SetStandBy;
        }

        bool standBy() const
        {
            return _standBy;
        }
    };

    class RunnerActionHandler : public core::WebServerActionHandler
    {
    public:
        SMART_PTR_T(RunnerActionHandler);

        QString name() const override
        {
            return "RunnerActionHandler";
        }

        bool match() override;
        void execute() override;
        QMutex* dataMutex();
    private:
        void addRunCommand(QJsonObject json);
        void addStopCommand(QJsonObject json);
        void addUpdateStatusCommand(QJsonObject json);
        void addSetTimeCommand(QJsonObject json);
        void addSetRangeCommand(QJsonObject json);
        void addSetStandByCommand(QJsonObject json);

        QQueue<RunnerCommand::SharedPtr_t> _commands;
        QMutex _dataMutex;
    };

    struct RunnerConfig
    {
        int webServerPort;
    };

    class Runner
    {
    public:
        SMART_PTR_T(Runner);
        Runner(RunnerConfig config);

        void run();
    private:
        WebServer::SharedPtr_t _webServer;
        RunnerActionHandler::SharedPtr_t _actionHandler;
    };
}
