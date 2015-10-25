#include "WebServer.h"
#include "common/Logger.h"

#include <mongoose.h>
#include <common/Exception.h>

void *serve(void *param) {
    auto server = static_cast<mg_server*>(param);
    while (true)
    {
        mg_poll_server(server, 1000);
    }
}

int ev_handler(mg_connection *connection, enum mg_event event) {
    auto webServerPtr = static_cast<core::WebServer**>(connection->server_param);
    if (*webServerPtr == nullptr)
    {
        return MG_FALSE;
    }

    auto webServer = *webServerPtr;
    auto handlers = webServer->handlers();
    switch (event) {
    case MG_AUTH:
        return MG_TRUE;
    case MG_REQUEST:
        sLogger.info(QString("New connection to '%1'.").arg(connection->uri));
        for (auto& handler : handlers)
        {
            handler->connection(connection);
            if (handler->match())
            {
                sLogger.info(QString("Found handler '%1' -> '%2'.").arg(connection->uri).arg(handler->name()));
                try
                {
                    handler->execute();
                }
                catch (common::Exception& ex)
                {
                    sLogger.error(QString("Handler error (common::Exception): %1").arg(ex.what()));
                }
                catch (std::exception& ex)
                {
                    sLogger.error(QString("Handler error (std::exception): %1").arg(ex.what()));
                }
                return MG_TRUE;
            }
        }
        sLogger.info(QString("No handlers found for url '%1'.").arg(connection->uri));
        return MG_FALSE;
    default:
        return MG_FALSE;
    }
}

core::WebServer::WebServer()
{
    _port = 8000;
    _thisRef = new WebServer*;
    *_thisRef = this;
}

core::WebServer::~WebServer()
{
    *_thisRef = nullptr;
}

void core::WebServer::addActionHandler(WebServerActionHandler::SharedPtr_t handler)
{
    _handlers.push_back(handler);
}

void core::WebServer::runAsync()
{
    mg_server* server = mg_create_server(_thisRef, ev_handler);
    mg_set_option(server, "listening_port", QString::number(_port).toLatin1().data());
    mg_start_thread(serve, server);
}