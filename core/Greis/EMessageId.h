#pragma once

namespace Greis
{
    struct EMessageId
    {
        enum Type
        {
            Unknown,
            AngularVelocity, // [AV] Angular Velocities
            AntName, // [AN] Antenna Names
            BaseInfo, // [BI] Base Station Information
            Baseline, // [BL] Baseline
            Baselines, // [bL] Attitude Baselines
            BeiDouAlm, // [CA] BeiDou Almanac
            BeiDouEphemeris, // [CN] BeiDou Ephemeris
            BeiDouIonoParams, // [CI] BeiDou Ionospheric Parameters
            BeiDouUtcParam, // [CU] BeiDou UTC Time Parameters
            CalBandsDelay, // [gR] Code Delays of Receiver RF Bands
            ClockOffsets, // [rT] Receiver Clock Offsets
            CNR, // [EC], [E1], [E2], [E3], [E5], [El]: SNR
            CNR4, // [CE], [1E], [2E], [3E], [5E], [lE]: SNR x 
            CompRawNavData, // [cd] BeiDou Raw Navigation Data
            CP, // [PC], [P1], [P2], [P3], [P5], [Pl]: Carrier Phases
            Dops, // [DP] Dilution of Precision (DOP)
            DP, // [DC], [D1], [D2], [D3], [D5], [Dl]: Doppler
            EpochEnd, // [||](EE) Epoch End
            EpochTime, // [::](ET) Epoch Time
            ER, // [ER] Error
            Event, // [==](EV) Event
            ExtEvent, // [XA], [XB] External Event
            FileId, // [JP] File Identifier
            Flags, // [FC],[F1],[F2],[F3],[F5],[Fl]: Signal Lock Loop Flags
            FullRotationMatrix, // [mR] Attitude Full Rotation Matrix
            GALAlm, // [EA] GALILEO Almanac
            GALEphemeris, // [EN] GALILEO Ephemeris
            GalRawNavData, // [ED] GALILEO Raw Navigation Data
            GalUtcGpsParam, // [EU] GALILEO UTC and GPS Time Parameters
            GeoPos, // [PG] Geodetic Position
            GeoVel, // [VG] Geodetic Velocity
            GLOAlmanac, // [NA] GLONASS Almanac
            GloDelays, // [gC], [g1], [g2], [g3]: GLONASS Delays
            GLOEphemeris, // [NE] GLONASS Ephemeris
            GloNavData, // [LD] GLONASS Raw Navigation Data (obsolete)
            GloRawNavData, // [lD] GLONASS Raw Navigation Data
            GLOTime, // [NT] GLONASS Time
            GloUtcGpsParam, // [NU] GLONASS UTC and GPS Time Parameters
            GPSAlm0, // [GA] GPS Almanac
            GPSEphemeris0, // [GE] GPS Ephemeris
            GpsNavData0, // [GD] GPS Raw Navigation Data (obsolete)
            GpsRawNavData0, // [gd] GPS Raw Navigation Data
            GPSTime, // [GT] GPS Time
            GpsUtcParam, // [UO] GPS UTC Time Parameters
            HeadAndPitch, // [ha] Heading and Pitch
            IAmp, // [ec], [e1], [e2], [e3], [e5]: Raw Inphases (I)
            InertialMeasurements, // [IM] Inertial Measurements
            IonoDelay, // [ID] Ionospheric Delays
            IonoParams0, // [IO] GPS Ionospheric Parameters
            Latency, // [LT] Message Output Latency
            LoggingHistory, // [LH] Logging History
            MsgFmt, // [MF] Messages Format
            NavStatus, // [SS] Satellite Navigation Status
            Params, // [PM] Parameters
            Pos, // [PO] Cartesian Position
            PosCompTime, // [PT] Time of Continuous Position Computation
            PosCov, // [SP] Position Covariance Matrix
            PosStat, // [PS] Position Statistics
            PosVel, // [PV] Cartesian Position and Velocity
            PosVelVector, // [rV] Receiver’s Position and Velocity
            PPSOffset, // [ZA], [ZB] PPS Offset
            PR, // [RC], [R1], [R2], [R3], [R5], [Rl]: Pseudoranges
            QAmp, // [qc], [q1], [q2], [q3], [q5]: Raw Quadratures (Q)
            QZSSAlm, // [QA] QZSS Almanac
            QZSSEphemeris, // [QE] QZSS Ephemeris
            QzssIonoParams, // [QI] QZSS Ionospheric Parameters
            QzssNavData, // [QD] QZSS Raw Navigation Data (obsolete)
            QzssRawNavData, // [qd] QZSS Raw Navigation Data
            QzssUtcParam, // [QU] QZSS UTC Time Parameters
            RangeResidual, // [rr] Satellite Range Residuals
            RawMeas, // [rM] Raw Measurements
            RCPRc1, // [cp],[1p],[2p],[3p],[5p],[lp]: Relative Carrier Phases
            RCPRC0, // [CP],[1P],[2P],[3P],[5P],[lP]: Relative Carrier Phases
            RcvBeiDouTimeOffset, // [CO] BeiDou to Receiver Time Offset
            RcvDate, // [RD] Receiver Date
            RcvGALTimeOffset, // [EO] GALILEO to Receiver Time Offset
            RcvGLOTimeOffset, // [NO] GLONASS to Receiver Time Offset
            RcvGPSTimeOffset, // [GO] GPS to Receiver Time Offset
            RcvOscOffs, // [OO] Oscillator Offset
            RcvQZSSTimeOffset, // [QO] QZSS to Receiver Time Offset
            RcvSBASTimeOffset, // [WO] SBAS to Receiver Time Offset
            RcvTime, // [~~](RT) Receiver Time
            RcvTimeAccuracy, // [BP] Rough Accuracy of Time Approximation
            RcvTimeOffsAtPPS, // [YA], [YB] Time Offset at PPS Generation Time
            RcvTimeOffset, // [TO] Reference Time to Receiver Time Offset
            RcvTimeOffsetDot, // [DO] Derivative of Receiver Time Offset
            RE, // [RE] Reply
            RefEpoch, // [rE] Reference Epoch
            Rms, // [SG] Position and Velocity RMS Errors
            RotationAngles, // [AR] Rotation Angles
            RotationMatrix, // [MR] Rotation Matrix
            RotationMatrixAndVectors, // [mr] Rotation Matrix and Vectors
            RPR, // [1R], [2R], [3R], [5R], [lR]: Relative Pseudoranges
            SatAzimuth, // [AZ] Satellite Azimuths
            SatElevation, // [EL] Satellite Elevations
            SatIndex, // [SI] Satellite Indices
            SatNumbers, // [NN] GLONASS Satellite System Numbers
            SBASAlmanac, // [WA] SBAS Almanac
            SBASEhemeris, // [WE] SBAS Ephemeris
            SbasRawNavData, // [WD] SBAS Raw Navigation Data
            SbasUtcParam, // [WU] SBAS UTC Time Parameters
            SC, // [CC],[C1],[C2],[C3],[C5],[Cl]: Smoothing Corrections
            SCP, // [pc], [p1], [p2], [p3], [p5], [pl]: Carrier Phases
            Security0, // [SE] Security
            Security1, // [SM] Security for [rM]
            SolutionTime, // [ST] Solution Time-Tag
            Spectrum0, // [sp] Spectrum
            Spectrum1, // [sP] Extended Spectrum
            SPR, // [rc], [r1], [r2], [r3], [r5], [rl]: Pseudoranges
            SRDP, // [1d], [2d], [3d], [5d], [ld]: Relative Doppler
            SRPR, // [1r], [2r], [3r], [5r], [lr]: Relative Pseudoranges
            SS, // [cc],[c1],[c2],[c3],[c5],[cl]: Smoothing Corrections
            TrackingTime, // [TT] CA/L1 Overall Continuous Tracking Time
            TrackingTimeCA, // [TC] CA/L1 Continuous Tracking Time
            Vel, // [VE] Cartesian Velocity
            VelCov, // [SV] Velocity Covariance Matrix
            VelocityResidual, // [vr] Satellite Velocity Residuals
            Wrapper // [>>] Wrapper
        };
    };
}
