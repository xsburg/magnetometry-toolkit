#pragma once

#include <QtCore/QString>

#include "IBinaryStream.h"
#include "MSeedRecord.h"

namespace core
{
    enum MSeedDataEncoding
    {
        Ascii = 0,
        Int16 = 1,
        Int32 = 3,
        Float32 = 4,
        Float64 = 5,
        Steim1 = 10,
        Steim2 = 11,
        Geoscope24 = 12,
        Geoscope163 = 13,
        Geoscope164 = 14,
        Cdsn = 16,
        Sro = 30,
        Dwwssn = 32
    };

    enum MSeedPackVerbose
    {
        None = 0,
        All = 3
    };

    class MSeedWriter
    {
    public:
        SMART_PTR_T(MSeedWriter);

        MSeedWriter(IBinaryStream::SharedPtr_t binaryStream) : _binaryStream(binaryStream)
        {
            _recordLength = 512;
            _encoding = MSeedDataEncoding::Steim2;
            _verbose = All;
        }

        inline MSeedDataEncoding encoding() const { return _encoding; }
        inline void encoding(const MSeedDataEncoding& encoding) { _encoding = encoding; }

        inline int recordLength() const { return _recordLength; }
        inline void recordLength(const int& recordLength) { _recordLength = recordLength; }

        inline MSeedPackVerbose verbose() const { return _verbose; }
        inline void verbose(const MSeedPackVerbose& verbose) { _verbose = verbose; }

        inline int packedRecords() const { return _packedRecords; }
        inline int packedSamples() const { return _packedSamples; }

        bool write(IntegerMSeedRecord::SharedPtr_t sampleRange);
        void close();
    private:
        IBinaryStream::SharedPtr_t _binaryStream;
        int _recordLength;
        int _packedRecords;
        int _packedSamples;
        MSeedDataEncoding _encoding;
        MSeedPackVerbose _verbose;
    };
}
