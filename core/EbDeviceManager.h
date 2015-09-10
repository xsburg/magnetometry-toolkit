// ***********************************************************************
// <author>Stephan Burguchev</author>
// <copyright company="Stephan Burguchev">
//   Copyright (c) Stephan Burguchev 2012-2015. All rights reserved.
// </copyright>
// <summary>
//   EbDeviceManager.h
// </summary>
// ***********************************************************************
#pragma once

#include "common/SmartPtr.h"

namespace core
{
    class EbDeviceManager
    {
    public:
        SMART_PTR_T(EbDeviceManager);

        void connect();
    };
}
