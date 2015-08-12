#include "Sedis.h"

int SedisHeader::samplingRate() const // in msec
{
    int msec;
    switch (_data->ConfigWord & 0x0F)
    {
    case 0x00:
        msec = 16;
        break;
    case 0x01:
        msec = 8;
        break;
    case 0x02:
        msec = 4;
        break;
    case 0x03:
        msec = 2;
        break;
    case 0x04:
        msec = 1;
        break;
    default:
        //assert(false);
        msec = 16;
    }
    if (lowSamplingRateUsed())
    {
        msec *= 2;
    }
    return msec;
}

int SedisHeader::channelsCount() const
{
    int ret = 0;
    for (int ch = 1; ch <= 6; ++ch)
    {
        ret += (int)channelEnabled(ch);
    }
    return ret;
}

SedisHeader::GPSData::GPSData(char* PositionStr, unsigned int Compass)
{
    // latitude
    char latitude[9] = { PositionStr[0], PositionStr[1], PositionStr[2], '.', PositionStr[3], PositionStr[4], PositionStr[5], PositionStr[6], '\0' };
    sscanf(latitude, "%f", &_latitude);
    // longitude
    char longitude[10] = { PositionStr[7], PositionStr[8], PositionStr[9], PositionStr[10], '.', PositionStr[11], PositionStr[12], PositionStr[13], PositionStr[14], '\0' };
    sscanf(longitude, "%f", &_longitude);
    // attitude
    char attitude[7] = { PositionStr[15], PositionStr[16], PositionStr[17], PositionStr[18], PositionStr[19], PositionStr[20] };
    sscanf(attitude, "%d", &_attitude);
    // GPS Source
    switch (PositionStr[21])
    {
    case '0':
        _gpsSource = GPS2D;
        break;
    case '1':
        _gpsSource = GPS3D;
        break;
    case '9':
        _gpsSource = GPSUnknown;
        break;
    default:
        cerr << "Invalid GPS source bit!";
        _gpsSource = GPSUnknown;
    }
    // Age of data
    switch (PositionStr[22])
    {
    case '2':
        _dataAge = FreshData;
        break;
    case '1':
        _dataAge = OldData;
        break;
    case '0':
        _dataAge = NotAvailable;
        break;
    default:
        cerr << "Invalid GPS AgeData bit!";
        _dataAge = NotAvailable;
    }
    cout << _latitude << endl << _longitude << endl << _attitude;
}

ostream& operator<<(ostream& os, const SedisHeader::GPSData& data)
{
    os << "{ " << "Latitude = " << data._latitude << ", Longitude = " << data._longitude << ", Attitude = " << data._attitude << ", GpsSource: ";
    switch (data._gpsSource)
    {
    case SedisHeader::GPSData::GPS2D:
        os << "'2D GPS'";
        break;
    case SedisHeader::GPSData::GPS3D:
        os << "'2D GPS'";
        break;
    case SedisHeader::GPSData::GPSUnknown:
        os << "'Unknows GPS'";
        break;
    }
    os << ", GPSDataAge: ";
    switch (data._dataAge)
    {
    case SedisHeader::GPSData::FreshData:
        os << "'FreshData'";
        break;
    case SedisHeader::GPSData::OldData:
        os << "'OldData'";
        break;
    case SedisHeader::GPSData::NotAvailable:
        os << "'NotAvailable'";
        break;
    }
    os << " }" << endl;
    return os;
}

ostream& operator<<(ostream& os, const SedisHeader& sedis)
{
    os << "SedisHeader\n{"
        << "\ndataHeader = " << sedis.dataHeader()
        << "\nheaderSize = " << sedis.headerSize()
        << "\nbytesOutputFormat = " << sedis.bytesOutputFormat()
        << "\ninternalOffsetUsed = " << sedis.internalOffsetUsed()
        << "\nlowPowerModeUsed = " << sedis.lowPowerModeUsed()
        << "\nlowSamplingRateUsed = " << sedis.lowSamplingRateUsed()
        << "\nsamplingRate = " << sedis.samplingRate()
        << "\nchannelEnabled = (";
    for (int i = 1; i <= 6; ++i)
    {
        os << (sedis.channelEnabled(i) ? "1 " : "0 ");
    }
    os << ")"
        << "\nchannelsCount = " << sedis.channelsCount()
        << "\nblockSamples = " << sedis.blockSamples()
        << "\nsampleBytes = " << sedis.sampleBytes()
        << "\nfirstSampleTime = " << sedis.firstSampleTime()
        << "batteryVoltage = " << sedis.batteryVoltage()
        << "\ntemperature = " << sedis.temperature()
        << "\ntemperature = " << sedis.temperature()
        << "\nrevisionNumber = '" << sedis.revisionNumber()
        << "\nADCSerialNumber = '" << sedis.ADCSerialNumber()
        << "\nnumberSV = '" << sedis.numberSV()
        << "\n...Skip drift, gpsTime and gpsLocation...\n}" << endl;
    return os;
}
