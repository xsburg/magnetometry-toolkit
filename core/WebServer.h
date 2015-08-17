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

namespace core
{
    class WebServer
    {
    public:
        SMART_PTR_T(WebServer);

        WebServer();

        inline int port() const { return _port; }
        inline void port(const int& port) { _port = port; }

        void runAsync();
    private:
        int _port;
    };
}
