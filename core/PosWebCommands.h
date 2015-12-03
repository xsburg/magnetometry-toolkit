// ***********************************************************************
// <author>Stepan Burguchev</author>
// <copyright company="Comindware">
//   Copyright (c) Comindware 2010-2015. All rights reserved.
// </copyright>
// <summary>
//   PosDeviceLogger.h
// </summary>
// ***********************************************************************

#pragma once

#include <stdint.h>
#include <QtCore>
#include "common/SmartPtr.h"

namespace core
{

    enum PosWebCommandType
    {
        Run,
        Stop,
        UpdateStatus,
        SetTime,
        SetRange,
        SetStandBy,
        RunDiagnostics,
        RunModeAutoTest,
        ApplyMSeedSettings
    };

    class PosWebCommand
    {
    public:
        SMART_PTR_T(PosWebCommand);

        explicit PosWebCommand()
        {
        }

        virtual ~PosWebCommand()
        {
        }

        virtual PosWebCommandType type() const = 0;
    };

    class PosRunWebCommand : public PosWebCommand
    {
        int _intervalMilliseconds;
        int _timeFixIntervalSeconds;
    public:
        PosWebCommandType type() const override
        {
            return PosWebCommandType::Run;
        }

        explicit PosRunWebCommand(int intervalMilliseconds, int timeFixIntervalSeconds)
            : _intervalMilliseconds(intervalMilliseconds),
              _timeFixIntervalSeconds(timeFixIntervalSeconds)
        {
        }

        int intervalMilliseconds() const
        {
            return _intervalMilliseconds;
        }

        int timeFixIntervalSeconds() const
        {
            return _timeFixIntervalSeconds;
        }
    };

    class PosStopWebCommand : public PosWebCommand
    {
    public:
        PosWebCommandType type() const override
        {
            return PosWebCommandType::Stop;
        }
    };

    class PosUpdateStatusWebCommand : public PosWebCommand
    {
    public:
        PosWebCommandType type() const override
        {
            return PosWebCommandType::UpdateStatus;
        }
    };

    class PosRunDiagnosticsWebCommand : public PosWebCommand
    {
    public:
        PosWebCommandType type() const override
        {
            return PosWebCommandType::RunDiagnostics;
        }
    };

    class PosRunModeAutoTestWebCommand : public PosWebCommand
    {
    public:
        PosWebCommandType type() const override
        {
            return PosWebCommandType::RunModeAutoTest;
        }
    };

    class PosApplyMSeedSettingsWebCommand : public PosWebCommand
    {
        MSeedSettings _settings;
    public:
        explicit PosApplyMSeedSettingsWebCommand(const MSeedSettings& settings)
            : _settings(settings)
        {
        }

        MSeedSettings settings() const
        {
            return _settings;
        }

        PosWebCommandType type() const override
        {
            return PosWebCommandType::ApplyMSeedSettings;
        }
    };

    class PosSetTimeWebCommand : public PosWebCommand
    {
        QDateTime _time;
    public:
        explicit PosSetTimeWebCommand(const QDateTime& qDateTime)
            : _time(qDateTime)
        {
        }

        QDateTime time() const
        {
            return _time;
        }

        PosWebCommandType type() const override
        {
            return PosWebCommandType::SetTime;
        }
    };

    class PosSetRangeWebCommand : public PosWebCommand
    {
        uint32_t _center;
    public:
        explicit PosSetRangeWebCommand(uint32_t uint32_t)
            : _center(uint32_t)
        {
        }

        PosWebCommandType type() const override
        {
            return PosWebCommandType::SetRange;
        }

        uint32_t center() const
        {
            return _center;
        }
    };

    class PosSetStandByWebCommand : public PosWebCommand
    {
        bool _standBy;
    public:
        explicit PosSetStandByWebCommand(bool standBy)
            : _standBy(standBy)
        {
        }

        PosWebCommandType type() const override
        {
            return PosWebCommandType::SetStandBy;
        }

        bool standBy() const
        {
            return _standBy;
        }
    };
}
