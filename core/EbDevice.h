// ***********************************************************************
// <author>Stephan Burguchev</author>
// <copyright company="Stephan Burguchev">
//   Copyright (c) Stephan Burguchev 2012-2015. All rights reserved.
// </copyright>
// <summary>
//   EbDevice.h
// </summary>
// ***********************************************************************
#pragma once

#include "common/SmartPtr.h"
#include <QtCore>
#include "SerialPortBinaryStream.h"
#include <common/BitConverter.h>
#include <QtSerialPort/QSerialPort>
#include "BufferedLogger.h"

namespace core
{
    class EbDeviceException : public common::Exception
    {
    public:
        EbDeviceException(const QString& message);
    };

    class EbDevice
    {
    public:
        SMART_PTR_T(EbDevice);

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

        explicit EbDevice(BufferedLogger::SharedPtr_t logger = BufferedLogger::SharedPtr_t());

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
        void sendAuto(int32_t freq /* -X Hz || 1/X mes/sec, X = [-5,-1]|[1,86400] */);

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
        QList<Sample> readAllSamples(int readTimeout = 1000);
        Sample parseSample(char* respPtr);
        Sample readOneSample();
        bool validateSample(const Sample& sample);
        
        void runDiagnosticSequence();
        void runTestAutoSequence();

        void waitForInputSilence(int maxWaitCicles = 10, int readTimeout = 1000);

    private:
        common::BitConverter _bitConverter;
        QSerialPort _serialPort;
        Mode _mode;
        BufferedLogger::SharedPtr_t _logger;
        static const int SerialPortReadBufferSize = 125000;
        static const int MessageMaxSize = 125000;
        void sendCommand(QByteArray command, int delayMilliseconds = 500, bool escape = true);
        QByteArray readLastResponseMessage(int readTimeout = 1000);
        QString readResponseString(int readTimeout = 1000);
        QList<QByteArray> readAllResponseMessages(int readTimeout = 1000);

        QByteArray escapeData(QByteArray data);
        QByteArray unescapeData(QByteArray data);
        void assertTrue(bool condition, QString failureComment);
        void log(common::LogLevel level, const QString& message);
        void logInfo(const QString& message);
        void logDebug(const QString& message);
        void logError(const QString& message);
    };
}
