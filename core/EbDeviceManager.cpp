#include "EbDeviceManager.h"
#include "common/Logger.h"
#include "SerialPortBinaryStream.h"
#include <common/InvalidOperationException.h>
#include <boost/exception/diagnostic_information.hpp>
#include <QtSerialPort/QSerialPort>

core::EbDeviceManager::EbDeviceManager() : EbDeviceManager(Mode::Text)
{
}

core::EbDeviceManager::EbDeviceManager(Mode mode) : _bitConverter(Common::BitConverter::EByteOrder::MostSignificantByte), _mode(mode)
{
}

void core::EbDeviceManager::connect()
{
    sLogger.Info("Before connect...");
    QSerialPort serialPort;
    serialPort.setPortName("/dev/ttyUSB0");
    serialPort.setBaudRate(QSerialPort::Baud9600);
    serialPort.setFlowControl(QSerialPort::NoFlowControl);
    serialPort.setParity(QSerialPort::NoParity);
    serialPort.setDataBits(QSerialPort::Data8);
    serialPort.setStopBits(QSerialPort::OneStop);
    serialPort.open(QIODevice::ReadWrite);
    serialPort.setBreakEnabled(false);
    sLogger.Info("Connected, writing command...");
    auto written = serialPort.write("\x05\x00", 2);
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
            auto err = serialPort.error();
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
    }
}

void core::EbDeviceManager::sendEnq()
{
    sendCommand("\x05");
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
            uint32_t bDateTime = dateTime.toMSecsSinceEpoch() / 1000;
            _bitConverter.ToByteArray(bDateTime, command.data() + 5);
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
    sendCommand("run");
}

void core::EbDeviceManager::sendAuto(uint32_t freq)
{
    switch (_mode)
    {
    case Text:
    {
        auto centerStr = QString::number(freq);
        sendCommand("auto " + centerStr.toLatin1());
        break;
    }
    case Binary:
    {
        auto command = QByteArray("auto xxxx");
        _bitConverter.ToByteArray(freq, command.data() + 5);
        sendCommand(command);
        break;
    }
    default:
        throw Common::InvalidOperationException();
    }
}

void core::EbDeviceManager::sendCommand(QByteArray command)
{
    command.append('\x00');
    _serialPort->write(command);
}
