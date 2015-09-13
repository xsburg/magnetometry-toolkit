// ***********************************************************************
// <author>Stephan Burguchev</author>
// <copyright company="Stephan Burguchev">
//   Copyright (c) Stephan Burguchev 2012-2015. All rights reserved.
// </copyright>
// <summary>
//   EbDeviceManager.h
// </summary>
// ***********************************************************************
#pragma once

#include "common/SmartPtr.h"
#include <QtCore>
#include "SerialPortBinaryStream.h"
#include <common/BitConverter.h>

namespace core
{
    class EbDeviceManager
    {
    public:
        SMART_PTR_T(EbDeviceManager);

        enum Mode
        {
            Text,
            Binary
        };


        EbDeviceManager();
        explicit EbDeviceManager(Mode mode);

        void connect();

        Mode mode() const { return _mode; }
        void set_mode(Mode mode) { _mode = mode; }

        void sendEnq();
        void sendNak();
        void sendAbout();
        void sendStandBy(bool enabled);
        void sendGetMode();
        void sendSetMode(Mode mode);
        void sendGetTime();
        void sendSetTime(QDateTime dateTime);
        void sendGetDate();
        void sendSetDate(QDateTime dateTime);
        void sendGetRange();
        void sendSetRange(uint32_t center);
        void sendRun();
        void sendAuto(uint32_t freq);
    private:
        Common::BitConverter _bitConverter;
        SerialPortBinaryStream::UniquePtr_t _serialPort;
        Mode _mode;

    private:
        void sendCommand(QByteArray command);
    };
}
