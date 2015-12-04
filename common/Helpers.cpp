#include "Helpers.h"

QString common::Helpers::toISODateWithMilliseconds(const QDateTime& dateTime)
{
    int msec = dateTime.toMSecsSinceEpoch() % 1000;
    return dateTime.toString(Qt::ISODate).replace("Z", QString(".%1Z").arg(msec, 3, 10, QLatin1Char('0')));
}
