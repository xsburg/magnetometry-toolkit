#include "Runner.h"
#include <common/NotImplementedException.h>
#include "RunnerCommands.h"
#include <common/Logger.h>

core::Runner::Runner(RunnerConfig config) : _config(config)
{
    _actionHandler = std::make_shared<RunnerActionHandler>();
    _webServer = std::make_shared<WebServer>();
    _webServer->port(config.webServerPort);
    _webServer->addActionHandler(_actionHandler);
}

void core::Runner::run()
{
    // Running commands aggregator in background thread
    _webServer->runAsync();
    
    // Creating a device
    auto device = std::make_shared<core::EbDevice>();
    device->connect(_config.devicePortName);
    device->runDiagnosticSequence();
    device->runTestAutoSequence();

    bool isRunning = false;
    while (true)
    {
        if (isRunning)
        {
            // receive and handle data

        }
        else
        {
            // sleep 1 second as a fallback for data logging
            QThread::sleep(1);
        }

        // run commands
        sLogger.Debug("Reading pending commands...");
        QMutexLocker lock(_actionHandler->dataMutex());
        while (!_actionHandler->commands().empty())
        {
            sLogger.Debug("Found a command...");
            auto cmd = _actionHandler->commands().dequeue();
            switch (cmd->type())
            {
            case Run:
                {
                    auto runCmd = std::static_pointer_cast<RunRunnerCommand>(cmd);
                    isRunning = true;
                    int runningIntervalMs = runCmd->intervalMilliseconds();
                    sLogger.Info(QString("Executing command RUN with { intervalMilliseconds: %1 }...").arg(runningIntervalMs));
                    device->sendAuto();
                    sLogger.Info(QString("Executed."));
                    break;
                }
            case Stop:
                isRunning = false;
                break;
            case UpdateStatus: break;
            case SetTime: break;
            case SetRange: break;
            case SetStandBy: break;
            default:
                throw Common::NotImplementedException();
            }
            sLogger.Debug("Done reading command.");
        }
    }
}
