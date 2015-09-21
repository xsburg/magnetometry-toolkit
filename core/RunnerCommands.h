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

#include <stdint.h>
#include <QtCore>
#include "common/SmartPtr.h"

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

    class RunRunnerCommand : public RunnerCommand
    {
        int _intervalMilliseconds;
    public:
        RunnerCommandType type() const override
        {
            return RunnerCommandType::Run;
        }

        explicit RunRunnerCommand(int intervalMilliseconds)
            : _intervalMilliseconds(intervalMilliseconds)
        {
        }

        int intervalMilliseconds() const
        {
            return _intervalMilliseconds;
        }
    };

    class StopRunnerCommand : public RunnerCommand
    {
    public:
        RunnerCommandType type() const override
        {
            return RunnerCommandType::Stop;
        }
    };

    class UpdateStatusRunnerCommand : public RunnerCommand
    {
    public:
        RunnerCommandType type() const override
        {
            return RunnerCommandType::UpdateStatus;
        }
    };

    class SetTimeRunnerCommand : public RunnerCommand
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

    class SetRangeRunnerCommand : public RunnerCommand
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

    class SetStandByRunnerCommand : public RunnerCommand
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
}
