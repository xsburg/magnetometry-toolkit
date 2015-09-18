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
    private:
        mg_connection* _connection;
    protected:
        virtual ~WebServerActionHandler()
        {
        }

        bool methodMatch(const QString& requestMethod /* GET, POST, PUT, DELETE */);
        bool exactMatch(const QString& url);
        bool regexMatch(const QRegularExpression& pattern);

    public:
        SMART_PTR_T(WebServerActionHandler);

        mg_connection* connection() const { return _connection; }
        void connection(mg_connection* connection) { _connection = connection; }

        virtual QString name() const { return "Undefined"; }
        virtual bool match() = 0;
        virtual void execute() = 0;
    };
}
