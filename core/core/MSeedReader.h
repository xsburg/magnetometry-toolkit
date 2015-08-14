// ***********************************************************************
// <author>Stephan Burguchev</author>
// <copyright company="Stephan Burguchev">
//   Copyright (c) Stephan Burguchev 2012-2015. All rights reserved.
// </copyright>
// <summary>
//   MSeedReader.h
// </summary>
// ***********************************************************************
#pragma once

#include "SampleRange.h"
#include "MSeedWriter.h"

namespace core
{
    class MSeedReader
    {
    public:
        SMART_PTR_T(MSeedReader);

        MSeedReader(QString fileName) : _fileName(fileName)
        {
        }

        inline MSeedPackVerbose verbose() const { return _verbose; }
        inline void verbose(const MSeedPackVerbose& verbose) { _verbose = verbose; }

        AbstractSampleRange::SharedPtr_t read();
    private:
        QString _fileName;
        MSeedPackVerbose _verbose;
    };
}
