#include "MSeedSink.h"
#include "ErrorManager.h"
#include "MSeedWriter.h"

#include <algorithm>
#include <cassert>
#include <boost/format.hpp>

using boost::format;

/*!
    \class MSeedSink

    \brief Класс MSeedSink позволяет осуществлять парсинг потока данных от SEDIS 
    и конвертацию его в MSeed записи, используя SedisMSeedWriter.

    Данные от SEDIS поступают в виде непрерывного потока данных. Сначала идет 80-битный
    заголовок с общей информацией, далее данные за одну минуту. После этого все повторяется.

    Класс принимает данные из потока, буферизирует минутные записи и передает их 
    в SedisMSeedWriter для конвертации в MSeed и записи в файл.

    Настройка класса SedisMSeedWriter осуществляется функцией configureWriter
    или в конструкторе, после чего можно сразу же подавать данные.

    Типичный пример использование:
    \code
    char* data;

    // ...

    MSeedSink sink("SK", "RU", "IF", "myfile");

    sink.write(data, partOneSize);
    sink.write(data + partOneSize, partTwoSize);
    \endcode

    Формат генерации выходных файлов (сколько MSeed записей в файле, принцип их именования, итд) 
    зависит от настроек внутреннего класса-парсера SedisMSeedWriter.
*/

/// Конструктор с настройкой парсера
/*!
    \sa configureWriter
*/
MSeedSink::MSeedSink(const string& location, const string& network, const string& station, const string& outputFilename, EWriteType type, const vector<string>& chNames)
{
    init();
    configureWriter(location, network, station, outputFilename, type);
}

/// Конструктор по умолчанию
/*!
    Перед подачей данных требуется настроить парсер функцией configureWriter

    \sa configureWriter
*/
MSeedSink::MSeedSink()
{
    init();
}

/// Настройка внутреннего парсера
/*!
    В случае, когда невозможно определить все параметры в момент создания объекта, данная функция позволяет настроить
    его позднее.

    \note Не следует изменять настройки парсера, если какие либо данные уже были поданы!
*/
void MSeedSink::configureWriter(const string& location, const string& network, const string& station, const string& outputFilename, EWriteType type, const vector<string>& chNames)
{
    _writer->setLocation(location);
    _writer->setNetwork(network);
    _writer->setStation(station);
    _writer->setFileName(outputFilename, (SedisMSeedWriter::EWriteType)type);
    if (!chNames.empty())
    {
        _writer->setChannelNames(chNames);
    } else {
        cout << "Warning: channel names is not set. Using standard names." << endl;
        vector<string> standardNames(6);
        standardNames[0] = "BHZ";
        standardNames[1] = "BHE";
        standardNames[2] = "BHS";
        standardNames[3] = "BHN";
        standardNames[4] = "BHW";
        standardNames[5] = "BLZ";
        _writer->setChannelNames(standardNames);
    }
}

void MSeedSink::init()
{
    _writer = new SedisMSeedWriter;
    _recordBuf.resize(_size = initBufSize);
    resetBuf();
}

MSeedSink::~MSeedSink()
{
    delete _writer;
}

void MSeedSink::addData(const char* data, std::streamsize count)
{
    if (_off + count > _size)
    {
        _recordBuf.resize(max(_off + count, _size * 2));
    }

    memcpy(&_recordBuf[_off], data, count);
    _off += count;
    _size = _recordBuf.size();
}

void MSeedSink::resetBuf()
{
    _off = 0;
    _recSize = -1;
}

int MSeedSink::recordSize()
{
    if (_recSize != -1)
        return _recSize;

    TSedisHeader* d_header = const_cast<TSedisHeader*>(reinterpret_cast<const TSedisHeader*>(&_recordBuf[0]));
    SedisHeader header(d_header);
    int dataSize = header.blockSamples() * header.sampleBytes();
    return _recSize = headerSize + dataSize;
}

/// Запись данных в буфер на обработку в MSeed
/*!
    Функция накапливает поданные данные. При накоплении целой SEDIS записи, 
    данные передаются на обработку в SedisMSeedWriter.
*/
std::streamsize MSeedSink::write(const char* s, std::streamsize n)
{
#ifdef DEBUG
    cout << "MSeedSink::write() enter: n = " << n << endl;
#endif
    // Return the number of characters consumed.
    int ret = n;

    while (n > 0)
    {
        if (_off + n < headerSize)
        {
            // недостаточно данных для начала записи
            addData(s,n);
            break;
        } else {
            if (_off < headerSize)
            {
                // дополнения буфера чтобы он содержал заголовок sedis
                int headerTailSize = headerSize - _off;
                addData(s,headerTailSize);
                s += headerTailSize;
                n -= headerTailSize;
                // в буфере хедер, проверка что это правильный хедер MSeed - по первым 12 байтам
                //vector<char> headerMark(12);
                //strncpy(&headerMark[0], "SeismicData\0", 12);
                // TODO: проверить действия когда данные в конце
                //_recordBuf.resize(29);
                //memcpy(&_recordBuf[0], "zz123456zSeismicData", 20);
                //_off = 20;
                char* headerMark = "SeismicData\0";
                BufIter begin = _recordBuf.begin();
                BufIter end = _recordBuf.begin() + _off;
                BufIter headerIt = std::search(begin, end, headerMark, headerMark+12);
                if (headerIt != begin) // Полученные данные не являются хедером, ищем начало хедера
                {
                    int skipped;
                    if (headerIt != end) // Начало хедера найдено, удаление мусора в начале
                    {
                        skipped = _off - std::distance(headerIt, end);
                        string warning(str(format("Garbage in data stream, skipping %1% bytes!") % skipped));
                        sErrorMgr.addError(warning);
                        cout << warning << endl;

                        _recSize = -1;
                        _off = std::distance(headerIt, end);
                        std::copy(headerIt, end, begin);
                        if (_off < headerSize)
                        {
                            continue;
                        }
                    } else { // Начало хедера не найдено, все мусор, кроме последних 11 байт - там может быть начало хедера (например zzSeismicDa)
                        skipped = _off - 11;
                        string warning(str(format("Garbage in data stream, skipping %1% bytes!") % skipped));
                        sErrorMgr.addError(warning);
                        cout << warning << endl;

                        _recSize = -1;
                        BufIter newBegin = end - 11;
                        _off = 11;
                        std::copy(newBegin, end, begin);
                        continue;
                    }
                }
            }
            int recSize = recordSize();
            if (_off + n < recSize)
            {
                // недостаточно данных для начала записи
                addData(s,n);
                break;
            } else {
                // данных достаточно.
                // дозапись в буфер текущей записи
                int recTailSize = recSize - _off;
                addData(s,recTailSize);
                s += recTailSize;
                n -= recTailSize;
                // ...конвертация и запись...
                _writer->addRecord(_recordBuf);
                resetBuf();
                // дозапись остального в следующей итерации
            }
        }
    }
    if (_off < headerSize)
    {
        cout << (format("MSeed Parser: received %1% bytes, buffered %2% bytes") % ret % _off) << endl;
    } else {
        cout << (format("MSeed Parser: received %1% bytes, progress %2%/%3%") % ret % _off % recordSize()) << endl;
    }
#ifdef DEBUG
    if (_off >= headerSize)
    {
        TSedisHeader* d_header = const_cast<TSedisHeader*>(reinterpret_cast<const TSedisHeader*>(&_recordBuf[0]));
        SedisHeader header(d_header);

        cout << header;
    }
    cout << endl;
#endif
    return ret;
}

bool assertHeader(const char* header)
{
     TSedisHeader* d_header = const_cast<TSedisHeader*>(reinterpret_cast<const TSedisHeader*>(header));
     return false;
}

/// Сброс буфера данных и парсера SedisMSeedWriter
/*!
    Все данные, которые не были переданы парсеру, будут уничтожены!

    \sa SedisMSeedWriter::reset
*/
void MSeedSink::reset()
{
    resetBuf();
    _writer->reset();
}
