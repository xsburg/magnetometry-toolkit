#ifndef MSEEDWRITER_H
#define MSEEDWRITER_H

#include <ctime>
#include <fstream>
#include <string>
#include <vector>
#include <cassert>
#include <libmseed.h>
#include <boost/format.hpp>

#include "Sedis.h"

using std::ofstream;
using std::string;
using std::vector;
using std::ios;
using boost::format;

static BTime tm_to_BTime(tm ctime)
{
    BTime btime;
    btime.year = 1900 + ctime.tm_year;
    btime.day = ctime.tm_yday + 1;
    btime.hour = ctime.tm_hour;
    btime.min = ctime.tm_min;
    btime.sec = ctime.tm_sec;
    btime.fract = 0;
    return btime;
}

typedef int int32;

// Функция для записи mseed Record в файл
static void recorder(char* record, int reclen, void* pvOutStream)
{
    ofstream* outstream = reinterpret_cast<ofstream*>(pvOutStream);
    outstream->write(record, reclen);
}

// при конвертации подставлять текущее время вместо времени из файла SEDIS
//#define WRITE_CURRENTTIME

class SedisMSeedWriter
{
public:
    SedisMSeedWriter();
    ~SedisMSeedWriter();

    enum EWriteType
    {
        FILE_SINGLE = 0,
        FILE_MULTI = 1
    };
    // шаблон имени для записываемого mseed файла (без расширения mseed и индекса '-%d')
    inline void setFileName(const string& filename, EWriteType type) { _filename = filename; _writeType = type; }
    string curFileName() const;
    // TODO: сейчас не используется. число файлов = число вызовов addRecord(...)
    /// в байтах, настоящий размер может отличатся, так как записи MSeed нельзя разделять в разные файлы.
    void setSplitSize(int size);
    /// Название сети. 2 символа.
    inline void setNetwork(const string& network) { _network = network; }
    /// Название станции. 5 символов.
    inline void setStation(const string& station) { _station = station; }
    /// Название локации. 2 символа.
    inline void setLocation(const string& location) { _location = location; }
    /// Название каналов - вектор из 6 строк
    inline void setChannelNames(const vector<string>& names) { assert(names.size() == 6); _chNames = names; }
    /// длина генерируемых записей mseed
    inline void setReclen(int len) { _recLen = len; }
    // алгоритм сжатия
    inline void setEncoding(int encoding) { _encoding = encoding; }
    // добавление записи SEDIS в файл
    void addRecord(const vector<char>& data);
    void reset();
private:
    string _network;
    string _station;
    string _location;
    string _filename;
    EWriteType _writeType;
    vector<string> _chNames; // названия каналов

    int _recLen;
    int _encoding;

    ofstream* _curFile;
    int _curFileIndex;
    
    void openFile();
    void closeFile();

    //MSRecord *msr;
};

#endif // MSEEDWRITER_H
