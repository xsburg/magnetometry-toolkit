#ifndef MSEEDSINK_H
#define MSEEDSINK_H

#include <iostream>
#include <string>
#include <vector>
#include <iosfwd>                          // streamsize
#include <boost/iostreams/operations.hpp>  // write

using std::string;
using std::vector;
using std::max;
using std::cout;
using std::endl;

namespace io = boost::iostreams;

//#define DEBUG

class SedisMSeedWriter;

class MSeedSink
{
public:
    typedef char          char_type; /*!< char_type */
    typedef io::sink_tag  category;  /*!< Категория устройства - Sink */

    // Тип записи в файл. Указывается название единственного выходного файла или название шаблона для набора файлов небольшого размера
    enum EWriteType
    {
        FILE_SINGLE = 0,
        FILE_MULTI = 1
    };

    MSeedSink(const string& location, const string& network, const string& station, const string& outputFilename, EWriteType type, const vector<string>& chNames = vector<string>());
    MSeedSink();
    ~MSeedSink();

    // настройка конвертора
    void configureWriter(const string& location, const string& network, const string& station, const string& outputFilename, EWriteType type, const vector<string>& chNames = vector<string>());

    // парсинг массива данных как mseed и его запись
    std::streamsize write(const char* s, std::streamsize n);

    // очистка буфера и сброс индекса у генерируемых файлов
    void reset();

private:
    // добавление данных к буферу
    void addData(const char* data, std::streamsize count);
    bool assertHeader(const char* header); // проверка хедера. На входе массив длиной 80 байт (и более)
    void resetBuf();
    // вычисление размера sedis записи на основе данных из ее заголовка
    int recordSize();
    void init();

// private data:
    typedef vector<char>::iterator BufIter;
    vector<char> _recordBuf;
    int _off, _size, _recSize; // смещение в буфере (количество правильных данных в нем), размер буфера (вектор.size()), размер текущей записи sedis (кэш данных из заголовка sedis)
    
    static const int headerSize = 80;
    static const int initBufSize = 4096;
    
    SedisMSeedWriter* _writer;
};

#endif // MSEEDSINK_H
