#ifndef SEDIS_H
#define SEDIS_H

#include <iostream>
#include <string>
#include <cassert>
#include <ctime>

using std::ostream;
using std::endl;
using std::string;
using std::cerr;
using std::cout;

//--------------------- Низкоуровненые структуры, используемые для начального чтения данных -------------------
typedef short int16;
typedef unsigned short uint16;

#pragma pack(push, 1)
/*!
    Структура времени в заголовке SEDIS
*/
struct TimeDate // 8 bytes
{
    char SEC;
    char MIN;
    char HOURS;
    char DAY;		// Day of week. Not used.
    char DATE;
    char MONTH;
    int16 YEAR;		// 2 bytes
    friend ostream& operator<<(ostream& os, const TimeDate& date)
    {
        os << "{ " << "Time = '" << (int)date.HOURS << ":" << (int)date.MIN << ":" << (int)date.SEC << "'";
        os << ", Day = " << (int)date.DAY << ", Date = " << (int)date.DATE << ", Month = " << (int)date.MONTH;
        os << ", Year = " << (int)date.YEAR << " }" << endl;
        return os;
    }
    tm toCTime() const
    {
        tm ctime;
        ctime.tm_year = YEAR - 1900;
        ctime.tm_mon = MONTH - 1;
        ctime.tm_mday = DATE;
        ctime.tm_hour = HOURS;
        ctime.tm_min = MIN;
        ctime.tm_sec = SEC;
        mktime(&ctime);
        return ctime;
    }
};

/*!
    Структура заголовка SEDIS в низкоуровневом представлении
*/
struct TSedisHeader // size 80 bytes
{
    char DataHeader[12];	    /*!< Data ID "SeismicData" 0 */
    char HeaderSize;		    /*!< size of header 80 bytes */
    char ConfigWord;		    /*!< see below */
    char ChannelBitMap;	        /*!< see below */
    uint16 BlockSamples;	    /*!< 2 bytes. Samples per one minute Block */
    char SampleBytes;		    /*!< Bytes per one sample */
    TimeDate SampleTime;	    /*!< time of first sample in the block */
    uint16 Bat;		            /*!< Battery voltage = 50/1024*Bat in Volts */
    uint16 Temp;		        /*!< temperature = ( 5000/1024*Temp - 600)/10 °C */
    char Rev;			        /*!< software revision number 2 means 1.05 */
    char Board;			        /*!< Serial number of ADC board. */
    char Reserved1;
// 33 bytes
    unsigned char NumberSV;	    /*!< number of usable satellites in Drift mesuarement */
    uint16 Drift;			    /*!< Drift / 32,768 = Drift correction in msec. */
    TimeDate SedisTime;
    TimeDate GPSTime;
    char Reserved2;
// 20+33=53 bytes
    bool Compass;		        /*!< Switcher. If 0xff then data below not valid, if 0 then position or if 1 then compass ASCII string. */
    char PositionStr[23];		/*!< ASCII string compass or position see below */
    char Reserved3[3];
};
#pragma pack(pop)
//-------------------------------------------------------------------------------------------------------------

/// Оболочка, интерпретирующая низкоуровневые данные заголовка SEDIS
class SedisHeader
{
public:
    /// Конструктор по умолчанию
    /*!
        \param data указатель на структуру с заголовком. Структура не копируется во внутреннее 
        представление и должна существовать пока существует данный объект.
    */
    SedisHeader(TSedisHeader* data)
    {
        // проверки по размеру
        assert(sizeof(TimeDate) == 8);
        assert(sizeof(TSedisHeader) == 80);

        _data = data;
    }

    /// Оповещение что это хедер - строка "SeismicData\0"
    inline string dataHeader() const { return string(_data->DataHeader); }
    /// Размер данного заголовка (80 байт)
    inline int headerSize() const { return int(_data->HeaderSize); }
    /// Формат данных одного канала в семпле - 3 или 4 байта
    inline int bytesOutputFormat() const { return (_data->ConfigWord & 0x80) ? 4 : 3; }
    /// ADC internal offset used. See CS5320/21/22 The OFST Pin, page 18.
    inline bool internalOffsetUsed() const { return (_data->ConfigWord & 0x40) ? true : false; }
    /// ADC operated in Low Power Mode.
    inline bool lowPowerModeUsed() const { return (_data->ConfigWord & 0x20) ? true : false; }
    /// Частота поступления семплов в 2 раза ниже (уже учитывается функцией samplingRate())
    inline bool lowSamplingRateUsed() const { return (_data->ConfigWord & 0x10) ? true : false; }
    /// Между каждым семплом данных samplingRate() msec
    int samplingRate() const;
    /// Канал numChannel включен (1,6)
    inline bool channelEnabled(int numChannel) const { assert(numChannel > 0 && numChannel < 7); return (_data->ChannelBitMap & (0x01 << (numChannel-1))) ? true : false; }
    /// Число активных каналов
    int channelsCount() const;
    /// маска активности каналов, младший бит = 1 канал итд
    inline char channelBitMap() const { return _data->ChannelBitMap; }
    /// Samples per one minute Block
    inline int blockSamples() const { return int(_data->BlockSamples); }
    /// Bytes per one sample
    inline int sampleBytes() const { return int(_data->SampleBytes); }
    /// time of first sample in the block
    inline TimeDate firstSampleTime() const { return _data->SampleTime; }
    /// Battery voltage in Volts
    inline double batteryVoltage() const { return 50.0 / 1024.0 * _data->Bat; }
    /// temperature °C
    inline double temperature() const { return (5000.0 / 1024.0 * _data->Temp - 600.0) / 10.0; }
    /// software revision number 2 means 1.05
    inline char revisionNumber() const { return _data->Rev; }
    /// Serial number of ADC board.
    inline char ADCSerialNumber() const { return _data->Board; }

    /// number of usable satellites in Drift mesuarement
    inline char numberSV() const { return _data->NumberSV; }
    /// Drift correction in msec.
    inline double drift() const { return _data->Drift / 32768.0; }
    inline TimeDate sedisTime() const { return _data->SedisTime; }
    inline TimeDate GPSTime() const { return _data->GPSTime; }

    // Time Correction
    // a) FullCorrection = GPSTime (seconds) – SedisTime (seconds) + Drift / 32768
    // b) TrueSedisTime = SedisTime + FullCorrection

    /// Класс для представления данных GPS
    class GPSData
    {
    public:
        enum ESourceOfGPS { GPS2D, GPS3D, GPSUnknown };
        enum EDataAge
        {
            FreshData,      /*!< '2' - Fresh data < 10 sec */
            OldData,        /*!< '1' - Old > 10sec */
            NotAvailable    /*!< '0' - Not available */
        };

        /// Конструктор
        /*!
            \param PositionStr текстовая строка C-style с данными.
            \param Compass число, определяющее формат строки.
        */
        GPSData(char* PositionStr, unsigned int Compass);
        friend ostream& operator<<(ostream& os, const GPSData& data);
        /// Широта
        inline float latitude() const { return _latitude; }
        /// Долгота
        inline float longitude() const { return _longitude; }
        /// Высота
        inline int attitude() const { return _attitude; }
        /// Тип GPS приемника
        inline ESourceOfGPS gpsSource() const { return _gpsSource; }
        /// "Возраст" данных
        inline EDataAge dataAge() const { return _dataAge; }
    private:
        float _latitude;
        float _longitude;
        int _attitude;
        ESourceOfGPS _gpsSource;
        EDataAge _dataAge;
    };

    /// Получить данные GPS
    inline GPSData gpsData() const { return GPSData(_data->PositionStr, _data->Compass); }

    friend ostream& operator<<(ostream& os, const SedisHeader& sedis);
private:
    TSedisHeader* _data;
};

#endif // SEDIS_H
