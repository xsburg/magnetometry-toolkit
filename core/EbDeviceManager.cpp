#include "EbDeviceManager.h"
#include "common/Logger.h"
#include "SerialPortBinaryStream.h"
#include "common/InvalidOperationException.h"
#include "common/NotSupportedException.h"

core::EbDeviceManager::EbDeviceManager() : EbDeviceManager(Mode::Text)
{
}

core::EbDeviceManager::EbDeviceManager(Mode mode) : _bitConverter(Common::BitConverter::EByteOrder::MostSignificantByte), _mode(mode)
{
}

void core::EbDeviceManager::connect(QString portName)
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
    sLogger.Debug("Done.");

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

void core::EbDeviceManager::sendEnq()
{
    sendCommand("\x05", 500, false);
}

void core::EbDeviceManager::sendNak()
{
    sendCommand("\x15");
}

void core::EbDeviceManager::sendAbout()
{
    sendCommand("about");
}

void core::EbDeviceManager::sendStandBy(bool enabled)
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

void core::EbDeviceManager::sendGetMode()
{
    sendCommand("mode");
}

void core::EbDeviceManager::sendSetMode(Mode mode)
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

void core::EbDeviceManager::sendGetTime()
{
    sendCommand("time");
}

void core::EbDeviceManager::sendSetTime(QDateTime dateTime)
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

void core::EbDeviceManager::sendGetDate()
{
    sendCommand("date");
}

void core::EbDeviceManager::sendSetDate(QDateTime dateTime)
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

void core::EbDeviceManager::sendGetRange()
{
    sendCommand("range");
}

void core::EbDeviceManager::sendSetRange(uint32_t center)
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

void core::EbDeviceManager::sendRun()
{
    sendCommand("run", 5000);
}

void core::EbDeviceManager::sendAuto(uint32_t freq)
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

QString core::EbDeviceManager::readEnq()
{
    return readResponseString();
}

QString core::EbDeviceManager::readAbout()
{
    return readResponseString();
}

bool core::EbDeviceManager::readStandBy()
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

core::EbDeviceManager::Mode core::EbDeviceManager::readGetMode()
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

core::EbDeviceManager::Mode core::EbDeviceManager::readSetMode()
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

QDateTime core::EbDeviceManager::readGetTime()
{
    auto data = readResponse();
    uint32_t unixtime = _bitConverter.GetUInt32(data);
    return QDateTime::fromTime_t(unixtime, Qt::UTC);
}

void core::EbDeviceManager::readSetTime()
{
    auto setTimeString = readResponseString();
    if (setTimeString == "set time ok")
    {
        return;
    }
    throw Common::Exception(QString("EbDevice command response: SetTime returned '%1'.").arg(setTimeString));
}

QDateTime core::EbDeviceManager::readGetDate()
{
    throw Common::NotSupportedException();
}

void core::EbDeviceManager::readSetDate()
{
    throw Common::NotSupportedException();
}

core::EbDeviceManager::RangeData core::EbDeviceManager::readGetRange()
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

core::EbDeviceManager::RangeData core::EbDeviceManager::readSetRange()
{
    return readGetRange();
}

core::EbDeviceManager::Sample core::EbDeviceManager::readSample()
{
    Sample data;
    auto response = readResponse();
    auto responsePtr = response.data();
    data.field = _bitConverter.GetInt32(responsePtr);
    data.qmc = _bitConverter.GetUInt16(responsePtr + 4);
    data.state = static_cast<SampleState>(_bitConverter.GetUInt8(responsePtr + 6));
    data.time = _bitConverter.GetInt32(responsePtr + 7);
    data.pph = _bitConverter.GetUInt8(responsePtr + 11);
    return data;
}

bool core::EbDeviceManager::validateSample(const Sample& sample)
{
    return sample.state == SampleState::Valid;
}

void core::EbDeviceManager::runDiagnosticSequence()
{
    sendEnq();
    auto enqData = readEnq();
    assertTrue(enqData.size() > 0, "ENQ data not empty");
}

void core::EbDeviceManager::sendCommand(QByteArray command, int delayMilliseconds, bool escape)
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

QByteArray core::EbDeviceManager::readResponse(qint64 maxlen)
{
    if (!_serialPort.waitForReadyRead(1000))
    {
        throw Common::Exception(QString("EbDevice read data error: waitForReadyRead timeout exceeded. Error string: %1.").arg(_serialPort.errorString()));
    }
    auto data = _serialPort.read(maxlen);
    if (data.size() != 0 && data.at(data.size() - 1) != '\0')
    {
        throw Common::Exception("EbDevice read data error: data block MUST end with \\0.");
    }
    return unescapeData(data);
}

QString core::EbDeviceManager::readResponseString(qint64 maxlen)
{
    auto response = readResponse(maxlen);
    auto responseString = QString(response);
    return responseString;
}

QByteArray core::EbDeviceManager::escapeData(QByteArray data)
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

QByteArray core::EbDeviceManager::unescapeData(QByteArray data)
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

void core::EbDeviceManager::assertTrue(bool condition, QString failureComment)
{
    if (!condition)
    {
        auto message = QString("EbDevice diagnostics failure: %1").arg(failureComment);
        sLogger.Error(message);
        throw Common::Exception(message);
    }
}
