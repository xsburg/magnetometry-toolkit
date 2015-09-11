#include "EbDeviceManager.h"
#include "common/Logger.h"
#include "SerialPortBinaryStream.h"

void core::EbDeviceManager::connect()
{
    sLogger.Info("Before connect...");
    _serialPort = std::make_unique<SerialPortBinaryStream>("/dev/ttyS3", 9600);
    sLogger.Info("Connected, writing command...");
    _serialPort->write("\x05\x00");
    sLogger.Info("Done, reading...");
    while (true)
    {
        auto data = _serialPort->read(1000);
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

void core::EbDeviceManager::sendCommand(QByteArray command)
{
    command.append('\x00');
    _serialPort->write(command);
}
