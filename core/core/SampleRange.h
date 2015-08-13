// ***********************************************************************
// <author>Stephan Burguchev</author>
// <copyright company="Stephan Burguchev">
//   Copyright (c) Stephan Burguchev 2012-2015. All rights reserved.
// </copyright>
// <summary>
//   ChannelData.h
// </summary>
// ***********************************************************************
#pragma once

#include <QtCore/QString>
#include <QtCore/QDateTime>
#include <QtCore/QVector>
#include <cstdint>

#include "Common/SmartPtr.h"
#include "IBinaryStream.h"
#include <Common/InvalidOperationException.h>

namespace Greis
{
    enum MSeedSampleType
    {
        Integer
    };

    class AbstractSampleRange
    {
    protected:
        virtual ~AbstractSampleRange()
        {
        }

    public:
        virtual MSeedSampleType getSampleType() const = 0;

        inline QString channelName() const { return _channelName; }
        inline void channelName(const QString& channelName)
        {
            if (channelName.size() > 10)
            {
                throw Common::InvalidOperationException("Channel name should not be longer than 10 characters.");
            }
            _channelName = channelName;
        }

        inline QString network() const { return _network; }
        inline void network(const QString& network)
        {
            if (network.size() > 10)
            {
                throw Common::InvalidOperationException("Network name should not be longer than 10 characters.");
            }
            _network = network;
        }

        inline QString station() const { return _station; }
        inline void station(const QString& station)
        {
            if (station.size() > 10)
            {
                throw Common::InvalidOperationException("Station name should not be longer than 10 characters.");
            }
            _station = station;
        }

        inline QString location() const { return _location; }
        inline void location(const QString& location)
        {
            if (location.size() > 10)
            {
                throw Common::InvalidOperationException("Location name should not be longer than 10 characters.");
            }
            _location = location;
        }

        inline QDateTime startTime() const { return _startTime; }
        inline void startTime(const QDateTime& startTime) { _startTime = startTime; }
    private:
        QString _channelName;
        QString _network;
        QString _station;
        QString _location;
        QDateTime _startTime;
    };

    class IntegerSampleRange : public AbstractSampleRange
    {
    public:
        SMART_PTR_T(IntegerSampleRange);

        virtual ~IntegerSampleRange()
        {
        }

        virtual MSeedSampleType getSampleType() const override
        {
            return MSeedSampleType::Integer;
        }

        inline const QVector<int32_t>& data() const { return _data; }
        inline QVector<int32_t>& data() { return _data; }

    private:
        QVector<int32_t> _data;
    };

    class MSeedWriter
    {
    public:
        SMART_PTR_T(MSeedWriter);

        MSeedWriter(IBinaryStream::SharedPtr_t binaryStream) : _binaryStream(binaryStream)
        {
            _recLen = 512;
            _encoding = DE_STEIM2;
        }

        void write(IntegerSampleRange::SharedPtr_t sampleRange);

    private:
        IBinaryStream::SharedPtr_t _binaryStream;
    };

#include <libmseed.h>

    inline void MSeedWriter::write(IntegerSampleRange::SharedPtr_t sampleRange)
    {
        // наполнения хедера mseed
        MSRecord* msr = msr_init(NULL);

        // общие для записей данные
        strcpy(msr->network, sampleRange->network().toLatin1().constData());
        strcpy(msr->station, sampleRange->station().toLatin1().constData());
        strcpy(msr->location, sampleRange->location().toLatin1().constData());

        msr->samprate = 1000.0 / header.samplingRate(); // число семплов в секунду

        msr->reclen = _recLen;
        msr->record = NULL;
        msr->encoding = _encoding;  // compression
        msr->byteorder = 1;         // big endian byte order

        msr->sampletype = 'i';      // declare type to be 32-bit integers
        //#endif
        //#ifdef MSR_PACK_FUNC
        strcpy(msr->channel, _chNames[channel - 1].c_str());

        msr->starttime = starttime; // для каждого канала начальное время одно и тоже
        msr->datasamples = &channelData[0]; // pointer to 32-bit integer data samples
        msr->numsamples = channelData.size();

        int psamples = 0;
        int precords = msr_pack(msr, &recorder, _curFile, &psamples, 1, verbose);
        if (precords == -1)
        {
            ms_log(2, "ERROR!!! Cannot pack records!\n");
            exit(1);
        }

        msr->datasamples = 0;
        msr_free(&msr);

        ms_log(0, "Packed %d samples into %d records\n", psamples, precords);
    }
}
