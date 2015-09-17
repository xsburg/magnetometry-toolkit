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
#include <QtSerialPort/QSerialPort>

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

        struct RangeData
        {
            int32_t minField;
            int32_t maxField;
        };

        enum SampleState
        {
            NUMERR = 0x01, // бит 0, значение поля не соответствует установленному рабочему поддиапазону
            TIMERR = 0x02, // бит 1, укорочение длительности сигнала
            NOISEERR = 0x04, // бит 2, низкое отношение сигнал/шум
            RESERVED = 0x08, // бит 3, зарезервирован
            OUTERR = 0x10, // бит 4, результат не попадает в пределы 20000-100000 нTл
            FAILED = 0x20, // бит 5, нет сигнала (измерение не проводилось)
            PWERR = 0x40, // бит 6, низкое напряжение питания (измерение не проводилось)
            Valid = 0x80, // бит 7, значение магнитного поля можно выводить на дисплей
            FatalError = 0x7F // 0x7F = Fatal Error
        };

        struct Sample
        {
            int32_t field; /* Field (pT) */
            uint16_t qmc;  /* Quality(pT) */
            SampleState state; /* State */
            QDateTime time;
        };

        EbDeviceManager();
        explicit EbDeviceManager(Mode mode);

        void connect(QString portName);

        Mode mode() const { return _mode; }

        void sendEnq();
        void sendNak();
        void sendAbout();
        void sendStandBy(bool enabled);
        void sendGetMode();
        void sendSetMode(Mode mode);
        void sendGetTime();
        void sendSetTime(QDateTime dateTime /* use UTC */);
        void sendGetDate();
        void sendSetDate(QDateTime dateTime /* use UTC */);
        void sendGetRange();
        void sendSetRange(uint32_t center);
        void sendRun();
        void sendAuto(uint32_t freq /* -X Hz || 1/X mes/sec, X = [-5,-1]|[1,86400] */);

        QString readEnq();
        QString readAbout();
        bool readStandBy();
        Mode readGetMode();
        Mode readSetMode();
        QDateTime readGetTime();
        void readSetTime();
        QDateTime readGetDate();
        void readSetDate();
        RangeData readGetRange();
        RangeData readSetRange();
        Sample readSample(int readTimeout = 1000);

        bool validateSample(const Sample& sample);
        
        void runDiagnosticSequence();
        void runTestAutoSequence();

    private:
        Common::BitConverter _bitConverter;
        QSerialPort _serialPort;
        Mode _mode;

        void sendCommand(QByteArray command, int delayMilliseconds = 500, bool escape = true);
        QByteArray readResponse(qint64 maxlen = 256, int readTimeout = 1000);
        QString readResponseString(qint64 maxlen = 256, int readTimeout = 1000);
        QByteArray escapeData(QByteArray data);
        QByteArray unescapeData(QByteArray data);
        void assertTrue(bool condition, QString failureComment);
    };
}
