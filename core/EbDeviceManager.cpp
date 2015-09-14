#include "EbDeviceManager.h"
#include "common/Logger.h"
#include "SerialPortBinaryStream.h"
#include <common/InvalidOperationException.h>

core::EbDeviceManager::EbDeviceManager() : EbDeviceManager(Mode::Text)
{
}

core::EbDeviceManager::EbDeviceManager(Mode mode) : _bitConverter(Common::BitConverter::EByteOrder::MostSignificantByte), _mode(mode)
{
}

void core::EbDeviceManager::connect()
{
    sLogger.Info("Before connect...");
    _serialPort = Common::make_unique<SerialPortBinaryStream>("/dev/ttyS3", 9600);
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
