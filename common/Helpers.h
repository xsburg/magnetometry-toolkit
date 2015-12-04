// ***********************************************************************
// <author>Stephan Burguchev</author>
// <copyright company="Stephan Burguchev">
//   Copyright (c) Stephan Burguchev 2012-2015. All rights reserved.
// </copyright>
// <summary>
//   Helpers.h
// </summary>
// ***********************************************************************
#pragma once

#include <QtCore/QString>
#include <QtCore/QDateTime>

namespace common
{
    class Helpers
    {
    public:
        static QString toISODateWithMilliseconds(const QDateTime& dateTime);
    };
}
