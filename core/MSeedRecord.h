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

#include <cstdint>

#include "common/SmartPtr.h"
#include <common/InvalidOperationException.h>

namespace core
{
    enum MSeedSampleType
    {
        Integer
    };

    class AbstractMSeedRecord
    {
    protected:
        virtual ~AbstractMSeedRecord()
        {
        }

    public:
        SMART_PTR_T(AbstractMSeedRecord);

        virtual MSeedSampleType getSampleType() const = 0;

        inline QString channelName() const { return _channelName; }
        inline void channelName(const QString& channelName)
        {
            if (channelName.size() > 10)
            {
                throw common::InvalidOperationException("Channel name should not be longer than 10 characters.");
            }
            _channelName = channelName;
        }

        inline QString network() const { return _network; }
        inline void network(const QString& network)
        {
            if (network.size() > 10)
            {
                throw common::InvalidOperationException("Network name should not be longer than 10 characters.");
            }
            _network = network;
        }

        inline QString station() const { return _station; }
        inline void station(const QString& station)
        {
            if (station.size() > 10)
            {
                throw common::InvalidOperationException("Station name should not be longer than 10 characters.");
            }
            _station = station;
        }

        inline QString location() const { return _location; }
        inline void location(const QString& location)
        {
            if (location.size() > 10)
            {
                throw common::InvalidOperationException("Location name should not be longer than 10 characters.");
            }
            _location = location;
        }

        inline double samplingRateHz() const { return _samplingRateHz; }
        inline void samplingRateHz(const double& samplingRateHz) { _samplingRateHz = samplingRateHz; }

        inline QDateTime startTime() const { return _startTime; }
        inline void startTime(const QDateTime& startTime) { _startTime = startTime; }
    private:
        QString _channelName;
        QString _network;
        QString _station;
        QString _location;
        QDateTime _startTime;
        double _samplingRateHz;
    };

    class IntegerMSeedRecord : public AbstractMSeedRecord
    {
    public:
        SMART_PTR_T(IntegerMSeedRecord);

        virtual ~IntegerMSeedRecord()
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
}
