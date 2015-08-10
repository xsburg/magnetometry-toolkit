#pragma once

namespace Greis
{
    struct ECustomTypeId
    {
        enum Type
        {
            Unknown,
            BandDelay,
            ClkOffs,
            ExtSpecData,
            GPSAlm1,
            GPSEphemeris1,
            GpsEphReqData,
            GpsNavData1,
            GpsRawNavData1,
            Header,
            IonoParams1,
            SlotRec,
            Smooth,
            SpecData,
            SvData0,
            SvData1,
            SvData2,
            SvDelay,
            UtcOffs
        };
    };
}
