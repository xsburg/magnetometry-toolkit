// ***********************************************************************
// <author>Stephan Burguchev</author>
// <copyright company="Stephan Burguchev">
//   Copyright (c) Stephan Burguchev 2012-2015. All rights reserved.
// </copyright>
// <summary>
//   WebServer.h
// </summary>
// ***********************************************************************
#pragma once

#include <common/SmartPtr.h>
#include "WebServerActionHandler.h"
#include <QtCore>

namespace core
{
    class WebServer
    {
    public:
        SMART_PTR_T(WebServer);

        WebServer();
        ~WebServer();

        inline int port() const { return _port; }
        inline void port(const int& port) { _port = port; }

        inline const QList<WebServerActionHandler::SharedPtr_t>& handlers() const { return _handlers; }

        void addActionHandler(WebServerActionHandler::SharedPtr_t handler);

        void runAsync();
    private:
        int _port;
        WebServer** _thisRef;
        QList<WebServerActionHandler::SharedPtr_t> _handlers;
    };
}
