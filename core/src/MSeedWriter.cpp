#include "MSeedWriter.h"

#include <libmseed.h>

namespace core
{
    inline BTime dateTimeToBTime(QDateTime dateTime)
    {
        auto date = dateTime.date();
        auto time = dateTime.time();
        BTime btime;
        btime.year = date.year();
        btime.day = date.dayOfYear();
        btime.hour = time.hour();
        btime.min = time.minute();
        btime.sec = time.second();
        btime.fract = time.msec();
        return btime;
    }

    static void binaryStreamRecorder(char* record, int reclen, void* pvOutStream)
    {
        auto binaryStream = reinterpret_cast<IBinaryStream*>(pvOutStream);
        binaryStream->write(record, reclen);
    }

    void MSeedWriter::close()
    {
        _binaryStream->close();
    }

    bool MSeedWriter::write(IntegerMSeedRecord::SharedPtr_t sampleRange)
    {
        // наполнения хедера mseed
        MSRecord* msr = msr_init(NULL);

        // общие для записей данные
        strcpy(msr->network, sampleRange->network().toLatin1().constData());
        strcpy(msr->station, sampleRange->station().toLatin1().constData());
        strcpy(msr->location, sampleRange->location().toLatin1().constData());
        strcpy(msr->channel, sampleRange->channelName().toLatin1().constData());

        msr->samprate = sampleRange->samplingRateHz();

        msr->reclen = _recordLength;
        msr->record = NULL;
        msr->encoding = _encoding;  // compression
        msr->byteorder = 1;         // big endian byte order

        BTime btime = dateTimeToBTime(sampleRange->startTime());
        msr->starttime = ms_btime2hptime(&btime);

        msr->sampletype = 'i';      // declare type to be 32-bit integers

        msr->datasamples = sampleRange->data().data();
        msr->numsamples = sampleRange->data().size();

        flag verbose = 3;
        _packedSamples = 0;
        _packedRecords = msr_pack(msr, &binaryStreamRecorder, _binaryStream.get(), &_packedSamples, 1, verbose);
        if (_packedRecords == -1)
        {
            return false;
        }

        msr->datasamples = NULL;
        msr_free(&msr);

        ms_log(0, "Packed %d samples into %d records\n", _packedSamples, _packedRecords);
        return true;
    }
}
