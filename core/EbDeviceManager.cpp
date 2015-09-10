#include "EbDeviceManager.h"
#include "common/Logger.h"
#include "SerialPortBinaryStream.h"

void core::EbDeviceManager::connect()
{
    sLogger.Info("Before connect...");
    auto serialPort = std::make_shared<SerialPortBinaryStream>("/dev/ttyS3", 9600);
    sLogger.Info("Connected, writing command...");
    serialPort->write("\x05\x00");
    sLogger.Info("Done, reading...");
    while (true)
    {
        auto data = serialPort->read(1000);
        sLogger.Info("------- DATA BLOCK ------");
        sLogger.Info(data);
        sLogger.Info("-------------------------");
    }
}