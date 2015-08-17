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

#include "MSeedRecord.h"
#include "MSeedWriter.h"

namespace core
{
    class MSeedReader
    {
    public:
        SMART_PTR_T(MSeedReader);

        MSeedReader(QString fileName);

        inline MSeedPackVerbose verbose() const { return _verbose; }
        inline void verbose(const MSeedPackVerbose& verbose) { _verbose = verbose; }

        QList<AbstractMSeedRecord::SharedPtr_t> readAll(bool *success = nullptr);
    private:
        QString _fileName;
        MSeedPackVerbose _verbose;
    };
}
