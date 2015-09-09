// ***********************************************************************
// <author>Stepan Burguchev</author>
// <copyright company="Comindware">
//   Copyright (c) Comindware 2010-2015. All rights reserved.
// </copyright>
// <summary>
//   WebServerActionHandler.h
// </summary>
// ***********************************************************************
#pragma once

#include <common/SmartPtr.h>
#include <QtCore>

struct mg_connection;

namespace core
{
    class WebServerActionHandler
    {
    protected:
        virtual ~WebServerActionHandler()
        {
        }

        bool exactMatch(mg_connection* connection, const QString& url);
        bool regexMatch(mg_connection* connection, const QRegularExpression& pattern);

    public:
        SMART_PTR_T(WebServerActionHandler);

        virtual QString name() const { return "Undefined"; }
        virtual bool match(mg_connection* connection) = 0;
        virtual void execute(mg_connection* connection) = 0;
    };
}
