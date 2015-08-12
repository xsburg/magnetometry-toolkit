#include "MSeedWriter.h"
#include "ErrorManager.h"
#include "MSeedSettings.h"

/*!
    \class SedisMSeedWriter

    \brief Класс SedisMSeedWriter совмещает в себе функционал по преобразованию
    одной SEDIS записи в набор записей MSeed заданной длины, а так же записи 
    полученных данных в файл.

    В процессе работы создаются файлы .mseed с преобразованными данными.
    Функции-члены setFileName и setSplitSize позволяют установить шаблон имени для 
    выходных файлов и средний их размер.

    Данные в формате SEDIS содержат недостаточно информации для создания mseed записей.
    Недостающие данные устанавливаются вручную соответствующими функциями.

    Типичный пример использование:
    \code
    vector<char> data;

    // ...

    SedisMSeedWriter writer;

    writer.setLocation(location);
    writer.setNetwork(network);
    writer.setStation(station);
    writer.setFileName(outFilename);

    writer.addRecord(data);
    \endcode
*/

/*! \fn void SedisMSeedWriter::setFileName(const string& filename)

    \brief Устанавливает шаблон имени для выходных файлов.

    Пример: при установке шаблона "myFile", на выходе будут файлы: "myFile-1.mseed", "myFile-2.mseed" итд.
*/

/*! \fn void SedisMSeedWriter::setEncoding(int encoding)

    \brief Устанавливает тип сжатия.

    Установка одного из возможных типов сжатия. 
    Возможны:
    \li DE_ASCII
    \li DE_INT16
    \li DE_INT32
    \li DE_FLOAT32
    \li DE_FLOAT64
    \li DE_STEIM1
    \li DE_STEIM2
    \li DE_GEOSCOPE24
    \li DE_GEOSCOPE163
    \li DE_GEOSCOPE164
    \li DE_CDSN
    \li DE_SRO
    \li DE_DWWSSN
*/

/// Конструктор по умолчанию
/*!
    По умолчанию длина MSeed записи составляет 512 байт, тип компрессии - DE_STEIM2.

    \sa setReclen, setEncoding
*/
SedisMSeedWriter::SedisMSeedWriter()
{
    _recLen = 512;
    _encoding = DE_STEIM2;

    /*_chNames.resize(6);
    _chNames[0] = "BHZ";
    _chNames[1] = "BHE";
    _chNames[2] = "BHS";
    _chNames[3] = "BHN";
    _chNames[4] = "BHW";
    _chNames[5] = "BLZ";*/
    _chNames = SettingsKeeper::getPtr()->channelNames();

    _curFileIndex = 0;
    _curFile = new ofstream;
    _curFile->exceptions(ios::badbit | ios::eofbit | ios::failbit);

    // test
    //MSRecord *msr = 0;
    /*msr = 0;
    MSTraceGroup *mstg = 0;
    MSTrace *mst;
    int retcode;

    int totalrecs  = 0;
    int totalsamps = 0;
    int packedsamples;
    int packedrecords;
    int lastrecord = 0;
    int iseqnum = 1;
    int verbose = 1;
    int reclen = 0;
    if ((retcode = ms_readmsr(&msr, "test.mseed", reclen, NULL, &lastrecord, 1, 1, verbose)) != MS_NOERROR)
    {
        std::cout << "nam pizdec!" << endl;
    }*/
}

/// Деструктор
SedisMSeedWriter::~SedisMSeedWriter()
{
    delete _curFile;
}
#define MSR_PACK_FUNC
//#define MSR_TRACE_FUNC
/// Преобразование одной записи SEDIS в формат MSeed
/*!
    \param data Вектор содержащий запись SEDIS

    В зависимости от числа каналов и настроек, создает одну и более записей 
    в формате MSeed.
*/
void SedisMSeedWriter::addRecord(const vector<char>& data)
{
    // получение хедера из sedis
    TSedisHeader d_head;
    memcpy(&d_head, &data[0], 80);
    SedisHeader header(&d_head);
    
#ifndef WRITE_CURRENTTIME
    tm ctime = header.firstSampleTime().toCTime();
#else
    tm ctime;
    time_t curtime = time(0);
    ctime = *localtime(&curtime);
#endif
    BTime btime = tm_to_BTime(ctime);
    hptime_t starttime = ms_btime2hptime(&btime);

/*
#ifdef MSR_PACK_FUNC
// наполнения хедера mseed
MSRecord* msr = msr_init(NULL);

// общие для записей данные
strcpy(msr->network, _network.c_str());
strcpy(msr->station, _station.c_str());
strcpy(msr->location, _location.c_str());

msr->samprate = 1000.0 / header.samplingRate(); // число семплов в секунду

msr->reclen = _recLen;
msr->record = NULL;
msr->encoding = _encoding;  // compression
msr->byteorder = 1;         // big endian byte order

msr->sampletype = 'i';      // declare type to be 32-bit integers
#endif
#ifdef MSR_TRACE_FUNC
// наполнения хедера mseed
MSTrace* mst = mst_init(NULL);

// общие для записей данные
strcpy(mst->network, _network.c_str());
strcpy(mst->station, _station.c_str());
strcpy(mst->location, _location.c_str());

mst->samprate = 1000.0 / header.samplingRate(); // число семплов в секунду

msr->reclen = _recLen;
msr->record = NULL;
msr->encoding = _encoding;  // compression
msr->byteorder = 1;         // big endian byte order

msr->sampletype = 'i';      // declare type to be 32-bit integers
#endif*/
    // данные для записи
    const int bytesOutputFormat = header.bytesOutputFormat(); // размер элемента данных в семпле
    const int bytesPerSample = header.sampleBytes();
    //int channelCount = header.sampleBytes() / bytesOutputFormat;

    int verbose = 0; // true

    openFile();

    vector<int32> channelData(header.blockSamples()); // буфер для данных по одному каналу
    int channelOffset = 0; // сдвиг данных по каналу относительно начала семпла
    for (int channel = 1; channel <= 6; ++channel)
    {
        if (header.channelEnabled(channel))
        {
            //cout << std::hex << (int)header.channelBitMap() << " channel=" << channel;
            channelData.assign(header.blockSamples(), 0);
            if (bytesOutputFormat == 3)
            {
                for (int sample = 0; sample < header.blockSamples(); ++sample)
                {
                    // копирование данных в 3 младших байта
                    const int sampleOffset = 80 + sample * bytesPerSample;
                    const unsigned char byte0 = (unsigned char)data[sampleOffset + channelOffset + 2];
                    const unsigned char byte1 = (unsigned char)data[sampleOffset + channelOffset + 1];
                    const unsigned char byte2 = (unsigned char)data[sampleOffset + channelOffset];
                    const unsigned char byte3 = ((byte2 & 0x80) != 0x00) ? 0xFF : 0x00;
                    channelData[sample] = 0x00000000 | (byte3 << 24)
                                                     | (byte2 << 16)
                                                     | (byte1 << 8)
                                                     | byte0;
                    //memcpy(((char*)&channelData[sample])+1, &data[80 + sample * bytesOutputFormat], bytesOutputFormat);
                }
            } else {
                for (int sample = 0; sample < header.blockSamples(); ++sample)
                {
                    channelData[sample] = *((int*)&data[80 + sample * bytesOutputFormat]);
                    //memcpy((char*)&channelData[sample], &data[80 + sample * bytesOutputFormat], bytesOutputFormat);
                }
            }

            // дебаг
            /*const int searchOffset = 500;
            if (channel == 1)
            {
                std::copy(channelData.begin() + searchOffset, channelData.begin() + searchOffset + 250, std::back_inserter(subseq));
            }
            if (channel == 2)
            {
                std::vector<int32>::iterator fnd = std::search(channelData.begin(), channelData.end(), subseq.begin(), subseq.end());
                if (fnd == channelData.end())
                {
                    cout << "JOPA!" << endl;
                } else {
                    cout << "PROFIT!" << 
                    " (FOUND AT = " << fnd - channelData.begin() << ") (SEQ AT = " << searchOffset << endl;
                }
            }*/

#ifdef MSR_PACK_FUNC
            // наполнения хедера mseed
            MSRecord* msr = msr_init(NULL);

            // общие для записей данные
            strcpy(msr->network, _network.c_str());
            strcpy(msr->station, _station.c_str());
            strcpy(msr->location, _location.c_str());

            msr->samprate = 1000.0 / header.samplingRate(); // число семплов в секунду

            msr->reclen = _recLen;
            msr->record = NULL;
            msr->encoding = _encoding;  // compression
            msr->byteorder = 1;         // big endian byte order

            msr->sampletype = 'i';      // declare type to be 32-bit integers
//#endif
//#ifdef MSR_PACK_FUNC
            strcpy(msr->channel, _chNames[channel-1].c_str());

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
#endif
#ifdef MSR_TRACE_FUNC
            // наполнения хедера mseed
            MSTrace* mst = mst_init(NULL);

            // общие для записей данные
            strcpy(mst->network, _network.c_str());
            strcpy(mst->station, _station.c_str());
            strcpy(mst->location, _location.c_str());

            mst->samprate = 1000.0 / header.samplingRate(); // число семплов в секунду

            //msr->reclen = _recLen;
            //msr->record = NULL;
            //msr->encoding = _encoding;  // compression
            //msr->byteorder = 1;         // big endian byte order

            mst->sampletype = 'i';      // declare type to be 32-bit integers
//#endif
//#ifdef MSR_PACK_FUNC
            strcpy(mst->channel, _chNames[channel-1].c_str());

            int32* dataPtr = (int32*) malloc(header.blockSamples()*sizeof(int32));
            memcpy(dataPtr,&channelData[0],channelData.size());

            mst->starttime = starttime; // для каждого канала начальное время одно и тоже
            mst->datasamples = dataPtr; // pointer to 32-bit integer data samples
            mst->numsamples = channelData.size();
            mst->samplecnt = channelData.size();
            
            int psamples = 0;
            int precords = mst_pack(mst, &recorder, _curFile, _recLen, DE_INT16, 1, &psamples, 1, verbose, NULL);
            //int precords = mst_pack(mst, &recorder, _curFile, _recLen, _encoding, 1, &psamples, 1, verbose, NULL);
            if (precords == -1)
            {
                ms_log(2, "ERROR!!! Cannot pack records!\n");
                exit(1);
            }

            mst->datasamples = 0;
            mst_free(&mst);
#endif

            ms_log(0, "Packed %d samples into %d records\n", psamples, precords);

            sErrorMgr.addError(str(format("File '%1%': Packed %2% samples into %3% records") % curFileName() % psamples % precords));            

            channelOffset += bytesOutputFormat; // увеличение относительного сдвига
        }
    }

    closeFile();

    /*msr->datasamples = 0;
    msr_free(&msr);*/
}

/// Сброс счетчика файлов
void SedisMSeedWriter::reset()
{
    _curFileIndex = 0;
}

void SedisMSeedWriter::openFile()
{
    switch (_writeType)
    {
    case FILE_SINGLE:
        if (!_curFile->is_open())
        {
            _curFile->open(_filename.c_str(), ios::out | ios::binary | ios::app);
        }
        break;
    case FILE_MULTI:
        _curFile->open(str(format("%1%-%2%.mseed") % _filename % ++_curFileIndex).c_str(), ios::out | ios::binary | ios::trunc);
        break;
    default:
        assert(false);
    }
}

void SedisMSeedWriter::closeFile()
{
    switch (_writeType)
    {
    case FILE_SINGLE:
        break;
    case FILE_MULTI:
        _curFile->close();
        break;
    default:
        assert(false);
    }
}

string SedisMSeedWriter::curFileName() const
{
    switch (_writeType)
    {
    case FILE_SINGLE:
        return _filename;
    case FILE_MULTI:
        return str(format("%1%-%2%.mseed") % _filename % _curFileIndex);
    default:
        assert(false);
        return string();
    }
}
