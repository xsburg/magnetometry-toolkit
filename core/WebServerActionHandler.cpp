#include "WebServerActionHandler.h"

#include <mongoose/mongoose.h>

bool core::WebServerActionHandler::methodMatch(const QString& requestMethod)
{
    return QString(connection()->request_method) == requestMethod;
}

bool core::WebServerActionHandler::exactMatch(const QString& testUrl)
{
    QString urlTrimmed = testUrl;
    if (testUrl.length() > 0 && testUrl.at(0) == '/')
    {
        urlTrimmed = testUrl.mid(1);
    }
    auto actualUrl = QString(connection()->uri);
    if (actualUrl.length() > 0 && actualUrl.at(0) == '/')
    {
        actualUrl = actualUrl.mid(1);
    }
    return actualUrl == urlTrimmed;
}

bool core::WebServerActionHandler::regexMatch(const QRegularExpression& pattern)
{
    return pattern.match(connection()->uri).hasMatch();
}
