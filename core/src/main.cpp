
#include "MSeedSettings.h"

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cassert>
#include <sstream>
#include <deque>
#include <algorithm>
#include <sys/stat.h>
#include <boost/format.hpp>

using std::cin;
using std::ofstream;
using std::string;
using std::vector;
using std::endl;
using std::ios;
using std::max;

using boost::format;

#include "MSeedWriter.h"
#include "MSeedSink.h"

class SedisFilesConverter
{
public:
    void configureWriter(const string& location, const string& network, const string& station, const string& outputFilename)
    {
        _writer.setLocation(location);
        _writer.setNetwork(network);
        _writer.setStation(station);
        _writer.setFileName(outputFilename, SedisMSeedWriter::FILE_SINGLE);
    }
    // Добавление файла через шаблон
    void convertFiles(const string& filenameTemplate)
    {
        // первый файл без суффикса
        string filename = filenameTemplate;
        std::ifstream imgfile(filename.c_str(), ios::in | ios::binary);
        int suffix = 1;
        int filesParsed = 1;
        int filesCount = checkTemplate(filename);
        while(true)
        {
            // чтение размера sedis записи и выделение буфера
            TSedisHeader d_header;
            int hreaded = imgfile.read((char*)&d_header, 80).gcount();
            if (hreaded == 0 && filesParsed == filesCount)
            {
                // нормальное завершение
                return;
            }
            SedisHeader header(&d_header);
            int datasize = header.blockSamples() * header.sampleBytes();
            vector<char> data(datasize + 80);
            // чтения всей sedis записи
            memcpy(&data[0], &d_header, 80);
            int readed = imgfile.read(&data[80], datasize).gcount();
            // если в файле не вся запись, попытка открыть следующий файл
            while (readed < datasize)
            {
                // открытие следующего файла
                imgfile.close();
                if (filesParsed == filesCount)
                {
                    // ошибка
                    return;
                }
                ++filesParsed;
                imgfile.seekg(0, ios::beg);
                imgfile.open(str(format("%1%-%2%") % filename % suffix).c_str(), ios::in | ios::binary);
                cout << imgfile.is_open() << imgfile.bad() << imgfile.fail() << imgfile.eof();
                int lastReaded = imgfile.read(&data[80+readed], datasize - readed).gcount();
                readed += lastReaded;
                ++suffix;
            }
            _writer.addRecord(data);
        }
    }
private:
    // Проверка шаблона входных файлов. Возвращает количество файлов по шаблону.
    int checkTemplate(const string& filename)
    {
        struct stat stFileInfo;
        int result = stat(filename.c_str(), &stFileInfo); // проверка файла-исключения
        int suffix = 0;
        int filesCount = 0;
        while (result == 0)
        {
            ++filesCount;
            result = stat(str(format("%1%-%2%") % filename % ++suffix).c_str(), &stFileInfo);
        }
        return filesCount;
    }

    SedisMSeedWriter _writer;
};

int main(int argc, char **argv)
{
    if (argc != 2)
    {
        cout << "I need filename!11" << endl;
        return 1;
    }

    cout << SettingsKeeper::getPtr()->location();
    cout << SettingsKeeper::getPtr()->filename();
    cout << SettingsKeeper::getPtr()->network();
    cout << SettingsKeeper::getPtr()->station();
    cout << (MSeedSink::EWriteType)SettingsKeeper::getPtr()->writeType();

    MSeedSink sink("SK", "RU", "IF", "myfile", MSeedSink::FILE_SINGLE);

    std::ifstream imgfile("station1_3ch8ms.img", ios::in | ios::binary);
    TSedisHeader d_header;
    imgfile.read((char*)&d_header, 80);
    SedisHeader header(&d_header);
    int datasize = header.blockSamples() * header.sampleBytes();
    vector<char> data(datasize + 80);
    memcpy(&data[0], &d_header, 80);
    imgfile.read(&data[80], datasize);

    /*const int searchOffset = 300;
    const int sampleSize = 9;
    
    vector<char> cmpPart;
    for (int i = 0; i < 50; ++i)
    {
        cmpPart.push_back(data[80+(searchOffset+i)*9]);
        cmpPart.push_back(data[80+(searchOffset+i)*9+1]);
        cmpPart.push_back(data[80+(searchOffset+i)*9+2]);
    }

    std::deque<char> cmpBuf;

    for (int i = 80+3; i < data.size(); i+=9)
    {
        /*if (cmpBuf.size() >= 50*3)
        {
            cmpBuf.pop_front();
            cmpBuf.pop_front();
            cmpBuf.pop_front();
        }
        if (80+(searchOffset+i)*9+3 >= data.size())
        {
            break;
        }
        cmpBuf.push_back(data[80+(searchOffset+i)*9+3]);
        cmpBuf.push_back(data[80+(searchOffset+i)*9+1+3]);
        cmpBuf.push_back(data[80+(searchOffset+i)*9+2+3]);* /

        if (data[i] == cmpPart[0] && data[i+1] == cmpPart[1] && data[i+2] == cmpPart[2])
        {
            std::cout << "NUMBERMATCH";
        }
    }*/

    sink.write(&data[0], data.size());

    cout << "Press \"any key\"(c):" << endl;
    cin.get();
    return 0;
}
