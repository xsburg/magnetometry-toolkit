// ***********************************************************************
// <author>Stepan Burguchev</author>
// <copyright company="Comindware">
//   Copyright (c) Comindware 2010-2015. All rights reserved.
// </copyright>
// <summary>
//   MSeedSink.h
// </summary>
// ***********************************************************************
#pragma once

#include "common/SmartPtr.h"
#include "MSeedRecord.h"

namespace core
{
    class MSeedWriter;

    class MSeedSink
    {
    public:
        SMART_PTR_T(MSeedSink);

        explicit MSeedSink(QString fileName);

        bool write(IntegerMSeedRecord::SharedPtr_t sampleRange);
        bool flush();

    private:
        std::shared_ptr<MSeedWriter> _writer;
    };
}
