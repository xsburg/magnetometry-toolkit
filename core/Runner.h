// ***********************************************************************
// <author>Stepan Burguchev</author>
// <copyright company="Comindware">
//   Copyright (c) Comindware 2010-2015. All rights reserved.
// </copyright>
// <summary>
//   Runner.h
// </summary>
// ***********************************************************************
#pragma once

#include "common/SmartPtr.h"
#include <QtCore/QString>
#include "Common.h"

namespace core
{
    struct RunnerConfig
    {
        int webServerPort;
        QString devicePortName;
        MSeedSettings msSettings;
    };

    class Runner
    {
    public:
        SMART_PTR_T(Runner);

        explicit Runner();
        void run(RunnerConfig config);
    };
}
