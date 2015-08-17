#include "WebServer.h"

#include <mongoose.h>
#include <QtCore/QString>

void *serve(void *param) {
    auto server = static_cast<mg_server*>(param);
    while (true)
    {
        mg_poll_server(server, 1000);
    }
}

int ev_handler(mg_connection *conn, enum mg_event ev) {
    switch (ev) {
    case MG_AUTH:
        return MG_TRUE;
    case MG_REQUEST:
        //((core::WebServer*)conn->server_param)->port()
        if (!strcmp(conn->uri, "/api/sum")) {
            //handle_restful_call(conn);
            return MG_TRUE;
        }
        return MG_FALSE;
        /*mg_send_file(conn, "index.html", s_no_cache_header);
        return MG_MORE;*/
    default:
        return MG_FALSE;
    }
}

core::WebServer::WebServer()
{
    _port = 8000;
}

void core::WebServer::runAsync()
{
    mg_server* server = mg_create_server(this, ev_handler);
    mg_set_option(server, "listening_port", QString::number(_port).toLatin1().data());
    mg_start_thread(serve, server);
}