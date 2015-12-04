// ***********************************************************************
// <author>Stepan Burguchev</author>
// <copyright company="Comindware">
//   Copyright (c) Comindware 2010-2015. All rights reserved.
// </copyright>
// <summary>
//   Thread.h
// </summary>
// ***********************************************************************
#pragma once

#include <QtCore/qthread.h>
#include "SmartPtr.h"
#include <functional>

namespace common
{
    class Thread : public QThread
    {
        Q_OBJECT

        std::function<void()> _functor;

        void run() override
        {
            _functor();
        }
    public:
        SMART_PTR_T(Thread);

        explicit Thread(std::function<void()> functor) : _functor(functor)
        {
        }
    };
}
