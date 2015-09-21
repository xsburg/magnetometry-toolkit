#include "EbDevice.h"
#include "common/Logger.h"
#include "SerialPortBinaryStream.h"
#include "common/InvalidOperationException.h"
#include "common/NotSupportedException.h"

core::EbDevice::EbDevice() : EbDevice(Mode::Text)
{
}

core::EbDevice::EbDevice(Mode mode) : _bitConverter(Common::BitConverter::EByteOrder::MostSignificantByte), _mode(mode)
{
}

void core::EbDevice::connect(QString portName)
{
    sLogger.Debug(QString("Connecting to port %1...").arg(portName));
    _serialPort.setPortName(portName);
    _serialPort.setBaudRate(QSerialPort::Baud9600);
    _serialPort.setFlowControl(QSerialPort::NoFlowControl);
    _serialPort.setParity(QSerialPort::NoParity);
    _serialPort.setDataBits(QSerialPort::Data8);
    _serialPort.setStopBits(QSerialPort::OneStop);
    _serialPort.open(QIODevice::ReadWrite);
    _serialPort.setBreakEnabled(false);
    sLogger.Debug("Connected, sending setMode : binary...");
    sendSetMode(Mode::Binary);
    _mode = readSetMode();
    sLogger.Debug("Done connecting.");

    /*auto written = serialPort.write("\x05\x00", 2);
    //serialPort.flush();
    while (!serialPort.waitForBytesWritten(1000))
    {
    sLogger.Info("Failed waiting!");
    auto err = serialPort.error();
    sLogger.Info(QString::number(err));
    }
    sLogger.Info(QString("written: %1").arg(written));
    QThread::sleep(1);
    sLogger.Info("Done, reading...");
    while (!serialPort.waitForReadyRead(100))
    {
    }
    while (true)
    {
    auto data = serialPort.read(1000);
    //sLogger.Info(QString("is readable: %1").arg(serialPort.isReadable()));
    //sLogger.Info(QString("bytes available: %1").arg(serialPort.bytesAvailable()));
    if (data.size() != 0)
    {
    sLogger.Info("------- DATA BLOCK ------");
    sLogger.Info(data);
    sLogger.Info("-------------------------");
    }
    else {
    //sLogger.Info("EMPTY");
    auto err = _serialPort.error();
    //sLogger.Info(QString::number(err));
    }
    }
    return;

    sLogger.Info("Before connect...");
    QSerialPort serialPort;
    serialPort.setPortName("/dev/ttyUSB0");
    serialPort.setBaudRate(QSerialPort::Baud9600);
    serialPort.setFlowControl(QSerialPort::NoFlowControl);
    serialPort.setParity(QSerialPort::NoParity);
    serialPort.setDataBits(QSerialPort::Data8);
    serialPort.setStopBits(QSerialPort::OneStop);
    serialPort.setBreakEnabled(false);
    serialPort.open(QIODevice::OpenModeFlag::ReadWrite);
    sLogger.Info("Connected, writing command...");
    auto written = serialPort.write("\x05\x00", 2);
    if (!serialPort.waitForBytesWritten(1000))
    {
    sLogger.Info("Failed waiting!");
    }
    sLogger.Info(QString("written: %1").arg(written));
    serialPort.flush();
    sLogger.Info("Done, reading...");
    while (true)
    {
    sLogger.Info(QString("is readable: %1").arg(serialPort.isReadable()));
    sLogger.Info(QString("bytes available: %1").arg(serialPort.bytesAvailable()));
    auto data = serialPort.read(1);
    sLogger.Info("------- DATA BLOCK ------");
    sLogger.Info("-------------------------");
    auto err = serialPort.error();
    sLogger.Info(QString::number(err));
    }


    return;
    try
    {
    sLogger.Info("Before connect...");
    _serialPort = Common::make_unique<SerialPortBinaryStream>("/dev/ttyS3", 9600);
    _serialPort->close();
    _serialPort->open();
    sLogger.Info("Connected, writing command...");
    _serialPort->write("\x05\x00");
    sLogger.Info("Done, reading...");
    QThread::sleep(1);
    while (true)
    {
    auto data = _serialPort->read(1);
    sLogger.Info("------- DATA BLOCK ------");
    sLogger.Info(data);
    sLogger.Info("-------------------------");
    }
    }
    catch (boost::system::system_error& e)
    {
    sLogger.Error("------- DEFAULT ERROR BLOCK ------");
    if (e.code().value() == 2) {
    sLogger.Error("Connection to receiver could not be made.");
    sLogger.Error("The application could not find the port specified.");
    sLogger.Error("It is ether wrong configuration or the receiver moved to another port.");
    }
    else {
    sLogger.Error("No connection to receiver, code: " + QString::number(e.code().value()));
    }
    sLogger.Error("------- NEW ERROR BLOCK ------");
    sLogger.Error(QString("Error: %1").arg(e.what()));
    auto info = boost::diagnostic_information(e);
    sLogger.Error(QString("Info: %1").arg(info.c_str()));
    sLogger.Error("------------------------------");
    }*/
}

void core::EbDevice::sendEnq()
{
    sendCommand("\x05", 500, false);
}

void core::EbDevice::sendNak()
{
    sendCommand("\x15");
}

void core::EbDevice::sendAbout()
{
    sendCommand("about");
}

void core::EbDevice::sendStandBy(bool enabled)
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

void core::EbDevice::sendGetMode()
{
    sendCommand("mode");
}

void core::EbDevice::sendSetMode(Mode mode)
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
        throw Common::InvalidOperationException();
    }
}

void core::EbDevice::sendGetTime()
{
    sendCommand("time");
}

void core::EbDevice::sendSetTime(QDateTime dateTime)
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
        throw Common::InvalidOperationException();
    }
}

void core::EbDevice::sendGetDate()
{
    sendCommand("date");
}

void core::EbDevice::sendSetDate(QDateTime dateTime)
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
                   throw Common::InvalidOperationException();
    }
    default:
        throw Common::InvalidOperationException();
    }
}

void core::EbDevice::sendGetRange()
{
    sendCommand("range");
}

void core::EbDevice::sendSetRange(uint32_t center)
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
        throw Common::InvalidOperationException();
    }
}

void core::EbDevice::sendRun()
{
    sendCommand("run", 5000);
}

void core::EbDevice::sendAuto(int32_t freq)
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
        throw Common::InvalidOperationException();
    }
}

QString core::EbDevice::readEnq()
{
    return readResponseString();
}

QString core::EbDevice::readAbout()
{
    return readResponseString();
}

bool core::EbDevice::readStandBy()
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
    throw Common::Exception(QString("EbDevice command response: StandBy returned '%1'.").arg(standByString));
}

core::EbDevice::Mode core::EbDevice::readGetMode()
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
    throw Common::Exception(QString("EbDevice command response: GetMode returned '%1'.").arg(modeString));
}

core::EbDevice::Mode core::EbDevice::readSetMode()
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
    throw Common::Exception(QString("EbDevice command response: SetMode returned '%1'.").arg(modeString));
}

QDateTime core::EbDevice::readGetTime()
{
    auto data = readResponse();
    uint32_t unixtime = _bitConverter.GetUInt32(data);
    return QDateTime::fromTime_t(unixtime, Qt::UTC);
}

void core::EbDevice::readSetTime()
{
    auto setTimeString = readResponseString();
    if (setTimeString == "set time ok")
    {
        return;
    }
    throw Common::Exception(QString("EbDevice command response: SetTime returned '%1'.").arg(setTimeString));
}

QDateTime core::EbDevice::readGetDate()
{
    throw Common::NotSupportedException();
}

void core::EbDevice::readSetDate()
{
    throw Common::NotSupportedException();
}

core::EbDevice::RangeData core::EbDevice::readGetRange()
{
    if (_mode != Mode::Binary)
    {
        throw Common::NotSupportedException();
    }
    RangeData data;
    auto response = readResponse();
    data.minField = _bitConverter.GetInt32(response);
    data.maxField = _bitConverter.GetInt32(response.data() + 4);
    return data;
}

core::EbDevice::RangeData core::EbDevice::readSetRange()
{
    return readGetRange();
}

core::EbDevice::Sample core::EbDevice::readSample(int readTimeout)
{
    Sample data;
    auto response = readResponse(256, readTimeout);
    auto responsePtr = response.data();
    data.field = _bitConverter.GetInt32(responsePtr);
    data.qmc = _bitConverter.GetUInt16(responsePtr + 4);
    data.state = static_cast<SampleState>(_bitConverter.GetUInt8(responsePtr + 6));
    auto time = _bitConverter.GetInt32(responsePtr + 7);
    auto pph = _bitConverter.GetUInt8(responsePtr + 11);
    auto dateTime = QDateTime::fromTime_t(time, Qt::UTC);
    dateTime = dateTime.addMSecs(pph * 10);
    data.time = dateTime;
    return data;
}

bool core::EbDevice::validateSample(const Sample& sample)
{
    return sample.state == SampleState::Valid;
}

void core::EbDevice::runDiagnosticSequence()
{
    sLogger.Info("Testing ENQ...");
    sendEnq();
    auto responseString = readEnq();
    assertTrue(responseString.size() > 0, "ENQ data is empty.");
    sLogger.Info("Done.");

    sLogger.Info("Testing about...");
    sendAbout();
    responseString = readAbout();
    assertTrue(responseString.size() > 0, "About text is empty.");
    sLogger.Info("Done.");

    sLogger.Info("Testing StandBy true...");
    sendStandBy(true);
    bool standBy = readStandBy();
    assertTrue(standBy, "StandBy must be true.");
    sLogger.Info("Done.");

    sLogger.Info("Testing StandBy false...");
    sendStandBy(false);
    standBy = readStandBy();
    assertTrue(!standBy, "StandBy must be false.");
    sLogger.Info("Done.");

    sLogger.Info("Testing getMode...");
    sendGetMode();
    auto mode = readGetMode();
    assertTrue(mode == Mode::Binary, "Mode must be binary");
    sLogger.Info("Done.");

    sLogger.Info("Testing setMode text...");
    sendSetMode(Mode::Text);
    mode = readSetMode();
    assertTrue(mode == Mode::Text, "Mode must be text");
    sLogger.Info("Done.");

    sLogger.Info("Testing setMode binary...");
    sendSetMode(Mode::Binary);
    mode = readSetMode();
    assertTrue(mode == Mode::Binary, "Mode must be binary again");
    sLogger.Info("Done.");

    sLogger.Info("Testing setTime...");
    auto expectedTime = QDateTime(QDate(2015, 9, 17), QTime(15, 15, 15), Qt::UTC);
    sendSetTime(expectedTime);
    readSetTime();
    sLogger.Info(QString("Set time: %1").arg(expectedTime.toString(Qt::ISODate)));
    sLogger.Info("Done.");

    sLogger.Info("Testing getTime...");
    sendGetTime();
    auto actualTime = readGetTime();
    sLogger.Info(QString("Got time: %1").arg(actualTime.toString(Qt::ISODate)));
    assertTrue(actualTime.date() == expectedTime.date(), "Date must be the same.");
    assertTrue(actualTime.time().hour() == expectedTime.time().hour(), "Hours must be the same.");
    assertTrue(actualTime.time().minute() == expectedTime.time().minute(), "Minutes must be the same.");
    sLogger.Info("Done.");

    sLogger.Info("Testing getRange...");
    sendGetRange();
    auto rangeData = readGetRange();
    sLogger.Info(QString("Got range: minField: %1, maxField: %2").arg(rangeData.minField).arg(rangeData.maxField));
    sLogger.Info("Done.");

    sLogger.Info("Testing setRange 25500...");
    sendSetRange(25550);
    rangeData = readSetRange();
    sLogger.Info(QString("Got range: minField: %1, maxField: %2").arg(rangeData.minField).arg(rangeData.maxField));
    assertTrue(rangeData.minField == 25000, "Range min != 25000.");
    assertTrue(rangeData.maxField == 26300, "Range max != 26300.");
    sLogger.Info("Done.");

    sLogger.Info("Testing setRange 55500...");
    sendSetRange(55500);
    rangeData = readSetRange();
    sLogger.Info(QString("Got range: minField: %1, maxField: %2").arg(rangeData.minField).arg(rangeData.maxField));
    assertTrue(rangeData.minField == 54400, "Range min != 54400.");
    assertTrue(rangeData.maxField == 56700, "Range max != 56700.");
    sLogger.Info("Done.");

    sLogger.Info("Testing run...");
    sendRun();
    auto sample = readSample();
    auto isValid = validateSample(sample);
    sLogger.Info(QString("Got sample: field: %1, time: %2.%3, state: 0x%4, qmc: %5, isValid: %6")
        .arg(sample.field).arg(sample.time.toString(Qt::ISODate)).arg(sample.time.toMSecsSinceEpoch() % 1000)
        .arg(sample.state, 2, 16).arg(sample.qmc).arg(isValid));
    assertTrue(sample.state != FatalError, "Errors if any are not fatal.");
    sLogger.Info("Done.");

    //sendNak();
    //sample = readSample();
}

void core::EbDevice::runTestAutoSequence()
{
    int intervalBetweenSamples = 1;
    sLogger.Info(QString("Testing auto mode, every %1 seconds...").arg(intervalBetweenSamples));
    sendAuto(intervalBetweenSamples);
    for (int i = 0; i < 10; i++)
    {
        auto sample = readSample((intervalBetweenSamples + 1) * 1000);
        auto isValid = validateSample(sample);
        sLogger.Info(QString("Got sample #%7: field: %1, time: %2.%3, state: 0x%4, qmc: %5, isValid: %6")
            .arg(sample.field).arg(sample.time.toString(Qt::ISODate)).arg(sample.time.toMSecsSinceEpoch() % 1000)
            .arg(sample.state, 2, 16).arg(sample.qmc).arg(isValid).arg(i + 1));
        assertTrue(sample.state != FatalError, "Errors if any are not fatal.");
    }
    sendEnq();
    auto responseString = readEnq();
    assertTrue(responseString.size() > 0, "ENQ data is empty.");
    sLogger.Info("Done.");
}

void core::EbDevice::sendCommand(QByteArray command, int delayMilliseconds, bool escape)
{
    if (escape)
    {
        command = escapeData(command);
    }
    command.append('\x00');
    int written = _serialPort.write(command);
    if (command.size() != written)
    {
        throw Common::Exception(QString("EbDevice command execution error: written size < command size. Error string: %1.").arg(_serialPort.errorString()));
    }
    if (!_serialPort.waitForBytesWritten(1000))
    {
        throw Common::Exception(QString("EbDevice command execution error: waitForBytesWritten timeout exceeded. Error string: %1.").arg(_serialPort.errorString()));
    }
    QThread::msleep(delayMilliseconds);
}

QByteArray core::EbDevice::readResponse(qint64 maxlen, int readTimeout)
{
    if (!_serialPort.waitForReadyRead(readTimeout))
    {
        throw Common::Exception(QString("EbDevice read data error: waitForReadyRead timeout exceeded. Error string: %1.").arg(_serialPort.errorString()));
    }
    auto data = _serialPort.read(maxlen);

    auto responses = data.split('\0');
    if (responses.size() > 1)
    {
        if (responses.last().size() == 0)
        {
            responses.pop_back();
        }
        if (responses.size() > 1)
        {
            sLogger.Debug(QString("EbDevice read notice: skipping %1 responses.").arg(responses.size() - 1));
        }
    }

    QByteArray responseData = responses.last();
    responseData += '\0';
    auto rawSize = responseData.size();
    responseData = unescapeData(responseData);
    sLogger.Debug(QString("EbDevice got response, raw size = %1, decoded size = %2...").arg(rawSize).arg(responseData.size()));
    return responseData;
}

QString core::EbDevice::readResponseString(qint64 maxlen, int readTimeout)
{
    auto response = readResponse(maxlen, readTimeout);
    auto responseString = QString(response);
    sLogger.Debug(QString("EbDevice got text response: '%1'").arg(responseString));
    return responseString;
}

QByteArray core::EbDevice::escapeData(QByteArray data)
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

QByteArray core::EbDevice::unescapeData(QByteArray data)
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

void core::EbDevice::assertTrue(bool condition, QString failureComment)
{
    if (!condition)
    {
        auto message = QString("EbDevice diagnostics failure: %1").arg(failureComment);
        sLogger.Error(message);
        throw Common::Exception(message);
    }
}
