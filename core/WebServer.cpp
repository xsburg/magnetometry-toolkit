#include "WebServer.h"
#include "common/Logger.h"

#include <mongoose.h>

void *serve(void *param) {
    auto server = static_cast<mg_server*>(param);
    while (true)
    {
        mg_poll_server(server, 1000);
    }
}

int ev_handler(mg_connection *connection, enum mg_event event) {
    sLogger.Info(QString("New connection to '%1'.").arg(connection->uri));
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
        for (auto& handler : handlers)
        {
            if (handler->match(connection))
            {
                sLogger.Info(QString("Found handler '%1' -> '%2'.").arg(connection->uri).arg(handler->name()));
                handler->execute(connection);
                return MG_TRUE;
            }
        }
        sLogger.Info(QString("No handlers found for url '%1'.").arg(connection->uri));
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