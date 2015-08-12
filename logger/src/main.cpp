#include <QtCore/QtCore>
#include <clocale>
#include <locale>
#include "Common/Logger.h"
#include "Common/SmartPtr.h"
#include "Common/Path.h"
#include "Common/Connection.h"

using namespace Common;

/*#include "Greis/DataChunk.h"
#include "Greis/SerialPortBinaryStream.h"
#include "Platform/SerialStreamReader.h"
#include "Platform/ServiceManager.h"
#include "Platform/ChainedSink.h"
#include "Greis/LoggingBinaryStream.h"
#include "Greis/FileBinaryStream.h"

using namespace Greis;
using namespace Platform;*/


#ifdef Q_OS_WIN
#include <windows.h> // for Sleep

#endif
void qSleep(int ms)
{
#ifdef Q_OS_WIN
    Sleep(uint(ms));
#else
    struct timespec ts = { ms / 1000, (ms % 1000) * 1000 * 1000 };
    nanosleep(&ts, NULL);
#endif
}

namespace jpslogd
{
    struct ReceiverInfo
    {
        ReceiverInfo(const QString& receiverId, const QString& receiverModel, const QString& receiverFw, const QString& receiverBoard)
            : receiverId(receiverId),
              receiverModel(receiverModel),
              receiverFw(receiverFw),
              receiverBoard(receiverBoard)
        {
        }

        QString receiverId;
        QString receiverModel;
        QString receiverFw;
        QString receiverBoard;
    };

    /*ReceiverInfo logDeviceInfo(SerialPortBinaryStream::SharedPtr_t& deviceBinaryStream)
    {
        auto serialStream = SerialStreamReader(deviceBinaryStream);
        // Get recevier data
        deviceBinaryStream->write("\nprint,/par/rcv/id\n");
        QString receiverId = serialStream.readLine();
        receiverId = receiverId.mid(6, -1);
        deviceBinaryStream->write("\nprint,/par/rcv/model\n");
        QString receiverModel = serialStream.readLine();
        receiverModel = receiverModel.mid(6, -1);
        deviceBinaryStream->write("\nprint,/par/rcv/ver/main\n");
        QString receiverFw = serialStream.readLine();
        receiverFw = receiverFw.mid(6, -1);
        deviceBinaryStream->write("\nprint,/par/rcv/ver/board\n");
        QString receiverBoard = serialStream.readLine();
        receiverBoard = receiverBoard.mid(6, -1);
        sLogger.Info("Connected device is " + receiverModel + ", board " + receiverBoard + " (ID:" + receiverId + ", FW:" + receiverFw + ")");
        return ReceiverInfo(receiverId, receiverModel, receiverFw, receiverBoard);
    }

    bool startLoop()
    {
        SerialPortBinaryStream::SharedPtr_t deviceBinaryStream;
        ChainedSink::UniquePtr_t dataCenterSink;
        ChainedSink::UniquePtr_t localSink;
        std::unique_ptr<DataChunk> dataChunk;

        try
        {
            std::string portName = sIniSettings.value("PortName", "COM3").toString().toStdString();
            unsigned int baudRate = sIniSettings.value("BaudRate", 1500000).toUInt();
            int inserterBatchSize = sIniSettings.value("inserterBatchSize", 250).toInt();
            int dataChunkSize = sIniSettings.value("dataChunkSize", 250).toInt();
            QString rawDataLogFile = sIniSettings.value("rawDataLogFile", "").toString();
            sLogger.Info("Connecting Javad receiver on " + QString::fromStdString(portName) + " at " + QString::number(baudRate) + "bps...");
            deviceBinaryStream = std::make_shared<SerialPortBinaryStream>(portName, baudRate);
            // Stop monitoring
            deviceBinaryStream->write("\r\ndm\n\r");
            deviceBinaryStream->purgeBuffers();

            auto receiverInfo = logDeviceInfo(deviceBinaryStream);

            // Setting parameters from [Receiver] section
            QStringList commands;
            auto keys = sIniSettings.settings()->allKeys();
            for (auto key : keys)
            {
                if (key.startsWith("Receiver/command"))
                {
                    commands.append(sIniSettings.value(key).toString());
                }
            }
            for (auto cmd : commands)
            {
                sLogger.Info(QString("Running a command: %1").arg(cmd));
                deviceBinaryStream->write(cmd.toLatin1());
                qSleep(500);
            }

            // message stream creation based on raw data logger option
            GreisMessageStream::SharedPtr_t messageStream;
            if (rawDataLogFile.isEmpty())
            {
                messageStream = std::make_shared<GreisMessageStream>(deviceBinaryStream, true, false);
            }
            else
            {
                auto proxyStream = std::make_shared<LoggingBinaryStream>(deviceBinaryStream, std::make_shared<FileBinaryStream>(rawDataLogFile, Create));
                messageStream = std::make_shared<GreisMessageStream>(proxyStream, true, false);
            }

            sLogger.Info("Configuring databases...");
            // Preparing the acquisition sink
            dataChunk = make_unique<DataChunk>();
            auto localConnection = Connection::FromSettings("LocalDatabase");
            if (localConnection->Driver == "" || localConnection->Hostname == "" || localConnection->Username == "")
            {
                sLogger.Fatal("Cannot configure local database, check configuraion.");
                throw new GreisException("Local database configuration missing.");
            }
            else
            {
                sLogger.Info("Using local database " + localConnection->DatabaseName + " on " + localConnection->Hostname);
            }

            auto remoteConnection = Connection::FromSettings("RemoteDatabase");
            if (remoteConnection->Driver != "")
            {
                dataCenterSink = make_unique<ChainedSink>(remoteConnection, inserterBatchSize, nullptr);
                localSink = make_unique<ChainedSink>(localConnection, inserterBatchSize, std::move(dataCenterSink));
                if (!dataCenterSink->IsValid())
                {
                    return true;
                }
                sLogger.Info("Using remote database " + localConnection->DatabaseName + " on " + localConnection->Hostname);
            }
            else
            {
                localSink = make_unique<ChainedSink>(localConnection, inserterBatchSize, nullptr);
            }
            if (!localSink->IsValid())
            {
                return true;
            }

            sLogger.Info("Configuring provisioning via local database...");
            auto serviceManager = make_unique<ServiceManager>(localConnection);
            // Set receiver properties
            serviceManager->ServiceStatus["receiverid"] = receiverInfo.receiverId;
            serviceManager->ServiceStatus["receiverfw"] = receiverInfo.receiverFw;
            serviceManager->ServiceStatus["receivermodel"] = receiverInfo.receiverModel;
            serviceManager->ServiceStatus["receiverboard"] = receiverInfo.receiverBoard;

            int msgCounter = 0;
            Message::UniquePtr_t msg;
            while ((msg = messageStream->Next()).get())
            {
                serviceManager->HandleMessage(msg.get());

                dataChunk->AddMessage(std::move(msg));
                if (msgCounter++ > dataChunkSize)
                {
                    auto dataChunk2 = make_unique<Greis::DataChunk>();
                    for (auto& msg : dataChunk->UnfinishedEpoch()->Messages)
                    {
                        dataChunk2->AddMessage(std::move(msg));
                    }
                    localSink->Handle(std::move(dataChunk));
                    dataChunk = std::move(dataChunk2);
                    msgCounter = 0;
                }

                if (msgCounter % 100 == 0)
                {
                    sLogger.Debug("Another 100 has been received.");

                    // Checking for the control commands
                    serviceManager->HandlePendingCommands();
                    if (serviceManager->IsRestartRequiredFlag)
                    {
                        serviceManager->IsRestartRequiredFlag = false;
                        localSink->Handle(std::move(dataChunk));
                        localSink->Flush();
                        return false;
                    }
                    if (serviceManager->IsShutdownRequiredFlag)
                    {
                        serviceManager->IsShutdownRequiredFlag = false;
                        localSink->Handle(std::move(dataChunk));
                        localSink->Flush();
                        return true;
                    }
                    if (serviceManager->IsPausedFlag)
                    {
                        const int sleepIntervalInMilliseconds = 1000;
                        localSink->Handle(std::move(dataChunk));
                        localSink->Flush();
                        dataChunk = make_unique<Greis::DataChunk>();
                        while (serviceManager->IsPausedFlag)
                        {
                            qSleep(sleepIntervalInMilliseconds);
                            serviceManager->HandlePendingCommands();
                            if (serviceManager->IsRestartRequiredFlag)
                            {
                                serviceManager->IsRestartRequiredFlag = false;
                                return false;
                            }
                            if (serviceManager->IsShutdownRequiredFlag)
                            {
                                serviceManager->IsShutdownRequiredFlag = false;
                                return true;
                            }
                        }
                    }
                }
            }

            localSink->Handle(std::move(dataChunk));
            localSink->Flush();
            return true;
        }
        catch (GreisException& ex)
        {
            sLogger.Error("An error has occurred: " + ex.what());

            if (localSink.get())
            {
                if (dataChunk.get())
                {
                    localSink->Handle(std::move(dataChunk));
                }
                localSink->Flush();
            }

            if (deviceBinaryStream.get() && deviceBinaryStream->isOpen())
            {
                deviceBinaryStream->write("\ndm\n");
                deviceBinaryStream->close();
            }

            return false;
        }
        catch (boost::system::system_error& bex)
        {
            if (bex.code().value() == 2)
            {
                sLogger.Error("Connection to receiver could not be made.");
                sLogger.Error("The application could not find the port specified.");
                sLogger.Error("It is ether wrong configuration or the receiver moved to another port.");
            }
            else
            {
                sLogger.Error("No connection to receiver, code: " + QString::number(bex.code().value()));
            }
            if (localSink.get())
            {
                if (dataChunk.get())
                {
                    localSink->Handle(std::move(dataChunk));
                }
                localSink->Flush();
            }
            return false;
        }
        catch (...)
        {
            sLogger.Error("Something bad has happened. Queueing restart.");
            return false;
        }
    }*/
}

int main(int argc, char** argv)
{
    try
    {
        QCoreApplication a(argc, argv);

        QTextCodec* codec = QTextCodec::codecForName("UTF-8");
        QTextCodec::setCodecForLocale(codec);

        sIniSettings.Initialize(Path::Combine(Path::ApplicationDirPath(), "config.ini"));
        sLogger.Initialize(sIniSettings.value("LogLevel", 5).toInt());

        sLogger.Debug("The following sqldrivers are available:");
        auto sqlDrivers = QSqlDatabase::drivers();
        for (auto it = sqlDrivers.begin(); it != sqlDrivers.end(); ++it)
        {
            QString sqlDriverName = *it;
            sLogger.Debug(sqlDriverName);
        }

        QStringList args = a.arguments();
        QString RootPassword = "";
        QRegExp rxArgRootPassword("--root-password=(\\S+)");
        QRegExp rxArgSetup("--setup");
        bool doSetup = false;

        for (int i = 1; i < args.size(); ++i)
        {
            if (rxArgRootPassword.indexIn(args.at(i)) != -1)
            {
                RootPassword = rxArgRootPassword.cap(1);
            }
            else if (rxArgSetup.indexIn(args.at(i)) != -1)
            {
                sLogger.Warn("Doing first time setup...");
                doSetup = true;
            }
        }
        if (doSetup)
        {
            if (RootPassword != "")
            {
                QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");
                db.setHostName("localhost");
                db.setUserName("root");
                db.setPassword(RootPassword);
                QFile baselineFile(Path::Combine(Path::ApplicationDirPath(), "baseline.sql"));
                if (!baselineFile.open(QIODevice::ReadOnly | QIODevice::Text))
                    return 0;
                QFile provisioningFile(Path::Combine(Path::ApplicationDirPath(), "jpslogd.sql"));
                if (!provisioningFile.open(QIODevice::ReadOnly | QIODevice::Text))
                    return 0;
                if (db.open())
                {
                    QSqlQuery query(db);
                    QTextStream in(&baselineFile);
                    QString sql = in.readAll();
                    QStringList sqlStatements = sql.split(';', QString::SkipEmptyParts);


                    query.exec("CREATE DATABASE `jpslogd`;");
                    query.exec("USE `jpslogd`;");
                    foreach(const QString& statement, sqlStatements)
                        {
                            if (statement.trimmed() != "")
                            {
                                if (!query.exec(statement))
                                sLogger.Error("An error occured during setup: " + query.lastError().text());
                            }
                        }
                    query.exec("GRANT ALL PRIVILEGES ON *.* TO 'jpslogd'@localhost");

                    QTextStream in2(&provisioningFile);
                    sql = in2.readAll();
                    sqlStatements = sql.split(';', QString::SkipEmptyParts);
                    foreach(const QString& statement, sqlStatements)
                        {
                            if (statement.trimmed() != "")
                            {
                                if (!query.exec(statement))
                                sLogger.Error("An error occured during setup: " + query.lastError().text());
                            }
                        }
                }
                return 0;
            }
            else
            {
                sLogger.Fatal("Please specify root password.");
            }
        }


        //while (!jpslogd::startLoop())
        {
            sLogger.Warn("An error occured, acquisition restart pending.");
            qSleep(1000);
        }

        return 0;
    }
    catch (Exception& e)
    {
        sLogger.Error(e.what());
        return 1;
    }
}
