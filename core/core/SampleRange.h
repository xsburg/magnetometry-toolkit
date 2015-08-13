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
        inline void channelName(const QString& channelName) { _channelName = channelName; }

        inline QString network() const { return _network; }
        inline void network(const QString& network) { _network = network; }

        inline QString station() const { return _station; }
        inline void station(const QString& station) { _station = station; }

        inline QString location() const { return _location; }
        inline void location(const QString& location) { _location = location; }

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
        }

        void write(IntegerSampleRange::SharedPtr_t sampleRange)
        {

        }

    private:
        IBinaryStream::SharedPtr_t _binaryStream;
    };
}
