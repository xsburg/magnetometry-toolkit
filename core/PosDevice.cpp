#include "PosDevice.h"
#include "common/Logger.h"
#include "SerialPortBinaryStream.h"
#include "common/InvalidOperationException.h"
#include "common/NotSupportedException.h"

core::PosDeviceException::PosDeviceException(const QString& message): common::Exception(message)
{
}

core::PosDevice::PosDevice(BufferedLogger::SharedPtr_t logger) :
    _bitConverter(common::BitConverter::EByteOrder::MostSignificantByte),
    _mode(Binary),
    _logger(logger)
{
}

void core::PosDevice::connect(QString portName)
{
    sLogger.debug(QString("Connecting to port %1...").arg(portName));
    _serialPort.setPortName(portName);
    _serialPort.setBaudRate(QSerialPort::Baud9600);
    _serialPort.setFlowControl(QSerialPort::NoFlowControl);
    _serialPort.setParity(QSerialPort::NoParity);
    _serialPort.setDataBits(QSerialPort::Data8);
    _serialPort.setStopBits(QSerialPort::OneStop);
    _serialPort.open(QIODevice::ReadWrite);
    _serialPort.setBreakEnabled(false);
    sLogger.debug("Connected, sending ENQ to interrupt auto mode that might be running...");
    sendEnq();
    waitForInputSilence();
    sLogger.debug("Sending setMode : binary...");
    sendSetMode(Mode::Binary);
    _mode = readSetMode();
    sLogger.debug("Done connecting.");
}

void core::PosDevice::sendEnq()
{
    sendCommand("\x05", 500, false);
}

void core::PosDevice::sendNak()
{
    sendCommand("\x15");
}

void core::PosDevice::sendAbout()
{
    sendCommand("about");
}

void core::PosDevice::sendStandBy(bool enabled)
{
    QByteArray command("standby ");
    if (enabled)
    {
        command.append("on");
    }
    else
    {
        command.append("off");
    }
    sendCommand(command);
}

void core::PosDevice::sendGetMode()
{
    sendCommand("mode");
}

void core::PosDevice::sendSetMode(Mode mode)
{
    switch (mode)
    {
    case Text:
        sendCommand("mode text");
        break;
    case Binary:
        sendCommand("mode binary");
        break;
    default:
        throw common::InvalidOperationException();
    }
}

void core::PosDevice::sendGetTime()
{
    sendCommand("time");
}

void core::PosDevice::sendSetTime(QDateTime dateTime)
{
    switch (_mode)
    {
    case Text:
    {
        auto timePart = dateTime.toString("hh:mm:ss");
        sendCommand("time " + timePart.toLatin1());
        break;
    }
    case Binary:
    {
        QByteArray command("time xxxx");
        uint32_t unixtime = dateTime.toTime_t();
        _bitConverter.ToByteArray(unixtime, command.data() + 5);
        sendCommand(command);
        break;
    }
    default:
        throw common::InvalidOperationException();
    }
}

void core::PosDevice::sendGetDate()
{
    sendCommand("date");
}

void core::PosDevice::sendSetDate(QDateTime dateTime)
{
    switch (_mode)
    {
    case Text:
    {
        auto timePart = dateTime.toString("MM:dd:yy");
        sendCommand("date " + timePart.toLatin1());
        break;
    }
    case Binary:
    {
        throw common::InvalidOperationException();
    }
    default:
        throw common::InvalidOperationException();
    }
}

void core::PosDevice::sendGetRange()
{
    sendCommand("range");
}

void core::PosDevice::sendSetRange(uint32_t center)
{
    switch (_mode)
    {
    case Text:
    {
        auto centerStr = QString::number(center);
        sendCommand("range " + centerStr.toLatin1());
        break;
    }
    case Binary:
    {
        auto command = QByteArray("range xxxx");
        _bitConverter.ToByteArray(center, command.data() + 6);
        sendCommand(command);
        break;
    }
    default:
        throw common::InvalidOperationException();
    }
}

void core::PosDevice::sendRun()
{
    sendCommand("run", 5000);
}

void core::PosDevice::sendAuto(int32_t freq)
{
    switch (_mode)
    {
    case Text:
    {
        auto centerStr = QString::number(freq);
        sendCommand("auto " + centerStr.toLatin1(), 5000);
        break;
    }
    case Binary:
    {
        auto command = QByteArray("auto xxxx");
        _bitConverter.ToByteArray(freq, command.data() + 5);
        sendCommand(command, 5000);
        break;
    }
    default:
        throw common::InvalidOperationException();
    }
}

QString core::PosDevice::readEnq()
{
    return readResponseString();
}

QString core::PosDevice::readAbout()
{
    return readResponseString();
}

bool core::PosDevice::readStandBy()
{
    auto standByString = readResponseString();
    if (standByString == "set standby on")
    {
        return true;
    }
    if (standByString == "set standby off")
    {
        return false;
    }
    throw PosDeviceException(QString("PosDevice command response: StandBy returned '%1'.").arg(standByString));
}

core::PosDevice::Mode core::PosDevice::readGetMode()
{
    auto modeString = readResponseString();
    if (modeString == "mode is text")
    {
        return Mode::Text;
    }
    if (modeString == "mode is binary")
    {
        return Mode::Binary;
    }
    throw PosDeviceException(QString("PosDevice command response: GetMode returned '%1'.").arg(modeString));
}

core::PosDevice::Mode core::PosDevice::readSetMode()
{
    auto modeString = readResponseString();
    if (modeString == "set text mode")
    {
        return Mode::Text;
    }
    if (modeString == "set binary mode")
    {
        return Mode::Binary;
    }
    throw PosDeviceException(QString("PosDevice command response: SetMode returned '%1'.").arg(modeString));
}

QDateTime core::PosDevice::readGetTime()
{
    auto data = readLastResponseMessage();
    uint32_t unixtime = _bitConverter.GetUInt32(data);
    return QDateTime::fromTime_t(unixtime, Qt::UTC);
}

void core::PosDevice::readSetTime()
{
    auto setTimeString = readResponseString();
    if (setTimeString == "set time ok")
    {
        return;
    }
    throw PosDeviceException(QString("PosDevice command response: SetTime returned '%1'.").arg(setTimeString));
}

QDateTime core::PosDevice::readGetDate()
{
    throw common::NotSupportedException();
}

void core::PosDevice::readSetDate()
{
    throw common::NotSupportedException();
}

core::PosDevice::RangeData core::PosDevice::readGetRange()
{
    if (_mode != Mode::Binary)
    {
        throw common::NotSupportedException();
    }
    RangeData data;
    auto response = readLastResponseMessage();
    data.minField = _bitConverter.GetInt32(response);
    data.maxField = _bitConverter.GetInt32(response.data() + 4);
    return data;
}

core::PosDevice::RangeData core::PosDevice::readSetRange()
{
    return readGetRange();
}

QList<core::PosDevice::Sample> core::PosDevice::readAllSamples(int readTimeout)
{
    auto responses = readAllResponseMessages(readTimeout);
    QList<Sample> result;
    for (auto& resp : responses)
    {
        auto respPtr = resp.data();
        result.push_back(parseSample(respPtr));
    }
    return result;
}

core::PosDevice::Sample core::PosDevice::parseSample(char* dataPtr)
{
    Sample data;
    data.field = _bitConverter.GetInt32(dataPtr);
    data.qmc = _bitConverter.GetUInt16(dataPtr + 4);
    data.state = static_cast<SampleState>(_bitConverter.GetUInt8(dataPtr + 6));
    auto time = _bitConverter.GetInt32(dataPtr + 7);
    auto pph = _bitConverter.GetUInt8(dataPtr + 11);
    auto dateTime = QDateTime::fromTime_t(time, Qt::UTC);
    dateTime = dateTime.addMSecs(pph * 10);
    data.time = dateTime;
    return data;
}

core::PosDevice::Sample core::PosDevice::readOneSample()
{
    auto resp = readLastResponseMessage();
    auto respPtr = resp.data();
    return parseSample(respPtr);
}

bool core::PosDevice::validateSample(const Sample& sample)
{
    return sample.state == SampleState::VALID;
}

void core::PosDevice::runDiagnosticSequence()
{
    logInfo("Testing ENQ...");
    sendEnq();
    auto responseString = readEnq();
    assertTrue(responseString.size() > 0, "ENQ data is empty.");
    logInfo("Done.");

    logInfo("Testing about...");
    sendAbout();
    responseString = readAbout();
    assertTrue(responseString.size() > 0, "About text is empty.");
    logInfo("Done.");

    logInfo("Testing StandBy true...");
    sendStandBy(true);
    bool standBy = readStandBy();
    assertTrue(standBy, "StandBy must be true.");
    logInfo("Done.");

    logInfo("Testing StandBy false...");
    sendStandBy(false);
    standBy = readStandBy();
    assertTrue(!standBy, "StandBy must be false.");
    logInfo("Done.");

    logInfo("Testing getMode...");
    sendGetMode();
    auto mode = readGetMode();
    assertTrue(mode == Mode::Binary, "Mode must be binary");
    logInfo("Done.");

    logInfo("Testing setMode text...");
    sendSetMode(Mode::Text);
    mode = readSetMode();
    assertTrue(mode == Mode::Text, "Mode must be text");
    logInfo("Done.");

    logInfo("Testing setMode binary...");
    sendSetMode(Mode::Binary);
    mode = readSetMode();
    assertTrue(mode == Mode::Binary, "Mode must be binary again");
    logInfo("Done.");

    logInfo("Testing setTime...");
    auto expectedTime = QDateTime(QDate(2015, 9, 17), QTime(15, 15, 15), Qt::UTC);
    sendSetTime(expectedTime);
    readSetTime();
    logInfo(QString("Set time: %1").arg(expectedTime.toString(Qt::ISODate)));
    logInfo("Done.");

    logInfo("Testing getTime...");
    sendGetTime();
    auto actualTime = readGetTime();
    logInfo(QString("Got time: %1").arg(actualTime.toString(Qt::ISODate)));
    assertTrue(actualTime.date() == expectedTime.date(), "Date must be the same.");
    assertTrue(actualTime.time().hour() == expectedTime.time().hour(), "Hours must be the same.");
    assertTrue(actualTime.time().minute() == expectedTime.time().minute(), "Minutes must be the same.");
    logInfo("Done.");

    logInfo("Testing getRange...");
    sendGetRange();
    auto rangeData = readGetRange();
    logInfo(QString("Got range: minField: %1, maxField: %2").arg(rangeData.minField).arg(rangeData.maxField));
    logInfo("Done.");

    logInfo("Testing setRange 25500...");
    sendSetRange(25550);
    rangeData = readSetRange();
    logInfo(QString("Got range: minField: %1, maxField: %2").arg(rangeData.minField).arg(rangeData.maxField));
    assertTrue(rangeData.minField == 25000, "Range min != 25000.");
    assertTrue(rangeData.maxField == 26300, "Range max != 26300.");
    logInfo("Done.");

    logInfo("Testing setRange 55500...");
    sendSetRange(55500);
    rangeData = readSetRange();
    logInfo(QString("Got range: minField: %1, maxField: %2").arg(rangeData.minField).arg(rangeData.maxField));
    assertTrue(rangeData.minField == 54400, "Range min != 54400.");
    assertTrue(rangeData.maxField == 56700, "Range max != 56700.");
    logInfo("Done.");

    logInfo("Testing run...");
    sendRun();
    auto sample = readOneSample();
    auto isValid = validateSample(sample);
    logInfo(QString("Got sample: field: %1, time: %2.%3, state: 0x%4, qmc: %5, isValid: %6")
        .arg(sample.field).arg(sample.time.toString(Qt::ISODate)).arg(sample.time.toMSecsSinceEpoch() % 1000)
        .arg(sample.state, 2, 16).arg(sample.qmc).arg(isValid));
    assertTrue(sample.state != FatalError, "Errors if any are not fatal.");
    logInfo("Done.");

    //sendNak();
    //sample = readSample();
}

void core::PosDevice::runTestAutoSequence()
{
    // sending AUTO command
    int intervalBetweenSamples = 1;
    logInfo(QString("Testing auto mode, every %1 seconds...").arg(intervalBetweenSamples));
    sendAuto(intervalBetweenSamples);
    // receiving data (at least 10 samples)
    for (int i = 0; i < 10; i++)
    {
        auto samples = readAllSamples();
        for (auto& sample : samples)
        {
            auto isValid = validateSample(sample);
            logInfo(QString("Got sample #%7: field: %1, time: %2.%3, state: 0x%4, qmc: %5, isValid: %6")
                .arg(sample.field).arg(sample.time.toString(Qt::ISODate)).arg(sample.time.toMSecsSinceEpoch() % 1000)
                .arg(sample.state, 2, 16).arg(sample.qmc).arg(isValid).arg(i + 1));
            assertTrue(sample.state != FatalError, "Errors if any are not fatal.");
        }
    }
    // stopping the madness
    sendEnq();
    waitForInputSilence();
    logInfo("Done.");
}

void core::PosDevice::sendCommand(QByteArray command, int delayMilliseconds, bool escape)
{
    if (escape)
    {
        command = escapeData(command);
    }
    command.append('\x00');
    int written = _serialPort.write(command);
    if (command.size() != written)
    {
        throw PosDeviceException(QString("PosDevice command execution error: `written size < command size`. Error string: `%1`.").arg(_serialPort.errorString()));
    }
    if (!_serialPort.waitForBytesWritten(1000))
    {
        throw PosDeviceException(QString("PosDevice command execution error: `waitForBytesWritten timeout exceeded`. Error string: `%1`.").arg(_serialPort.errorString()));
    }
    QThread::msleep(delayMilliseconds);
}

QByteArray core::PosDevice::readLastResponseMessage(int readTimeout)
{
    auto messages = readAllResponseMessages(readTimeout);
    if (messages.size() == 0)
    {
        throw PosDeviceException(QString("PosDevice read data error: `no messages in input stream`."));
    }
    if (messages.size() > 1)
    {
        logDebug(QString("PosDevice read notice: skipping %1 response messages.").arg(messages.size() - 1));
    }
    return messages.last();
}

QList<QByteArray> core::PosDevice::readAllResponseMessages(int readTimeout)
{
    logDebug(QString("PosDevice is reading pending response messages..."));
    QList<QByteArray> result;
    if (!_serialPort.waitForReadyRead(readTimeout))
    {
        logDebug(QString("PosDevice read data suspicious behavior: `waitForReadyRead timeout exceeded`. Error string: %1.").arg(_serialPort.errorString()));
        return result;
    }
    auto data = _serialPort.read(SerialPortReadBufferSize);

    auto responseMessages = data.split('\0');
    if (responseMessages.size() == 0)
    {
        return result;
    }

    for (auto& encodedMsg : responseMessages)
    {
        if (encodedMsg.size() == 0)
        {
            continue;
        }
        if (encodedMsg.size() > MessageMaxSize)
        {
            throw PosDeviceException(QString("PosDevice got a response message with invalid size (>%2 bytes). Actual size: %1.").arg(encodedMsg.size()).arg(MessageMaxSize));
        }
        encodedMsg += '\0';
        QByteArray responseMsg = unescapeData(encodedMsg);
        logDebug(QString("PosDevice got response message, raw size = %1, decoded size = %2...").arg(encodedMsg.size()).arg(responseMsg.size()));
        result.push_back(responseMsg);
    }

    logDebug(QString("PosDevice response messages: %1 messages read in total.").arg(result.size()));
    return result;
}

void core::PosDevice::waitForInputSilence(int maxWaitCicles, int readTimeout)
{
    int counter = 0;
    while (true)
    {
        auto messages = readAllResponseMessages();
        if (messages.size() == 0)
        {
            break;
        }
        counter++;
        if (counter > 10)
        {
            // We can't wait forever: something went wrong and we can't cope with it
            throw common::Exception("Failed to stop data acquisition. The device possibly stuck and must be rebooted via power cord.");
        }
        QThread::msleep(100);
    }
}

QString core::PosDevice::readResponseString(int readTimeout)
{
    auto response = readLastResponseMessage(readTimeout);
    auto responseString = QString(response);
    logDebug(QString("PosDevice got text response message: '%1'").arg(responseString));
    return responseString;
}

QByteArray core::PosDevice::escapeData(QByteArray data)
{
    QByteArray result;
    for (int i = 0; i < data.size(); i++)
    {
        uint8_t c = data.at(i);
        if (c < 0x20 || c == 0x1A)
        {
            result.append(0x1A);
            result.append(c + 0x80);
        }
        else
        {
            result.append(c);
        }
    }
    return result;
}

QByteArray core::PosDevice::unescapeData(QByteArray data)
{
    bool escaped = false;
    QByteArray result;
    for (int i = 0; i < data.size(); i++)
    {
        uint8_t c = data.at(i);
        if (c == 0x1A)
        {
            escaped = true;
        }
        else if (escaped)
        {
            escaped = false;
            result.append(c - 0x80);
        }
        else
        {
            result.append(c);
        }
    }
    return result;
}

void core::PosDevice::assertTrue(bool condition, QString failureComment)
{
    if (!condition)
    {
        auto message = QString("PosDevice diagnostics failure: %1").arg(failureComment);
        logError(message);
        throw PosDeviceException(message);
    }
}

void core::PosDevice::log(common::LogLevel level, const QString& message)
{
    if (_logger.get())
    {
        _logger->write(level, message);
    }
    sLogger.write(level, message);
}

void core::PosDevice::logInfo(const QString& message)
{
    log(common::Info, message);
}

void core::PosDevice::logDebug(const QString& message)
{
    log(common::Debug, message);
}

void core::PosDevice::logError(const QString& message)
{
    log(common::Error, message);
}
