#ifndef ERRORMANAGER_H
#define ERRORMANAGER_H

//#include <boost/date_time.hpp>
#include <boost/shared_ptr.hpp>
#include <ctime>
#include <fstream>
#include <string>
#include <cassert>
#include <iostream>

using std::string;
using std::cout;
//using namespace boost::posix_time;
//using namespace boost::gregorian;

#include "MSeedSettings.h"

class ErrorManager
{
public:
    void addError(const string& text)
    {
        if (!SettingsKeeper::getPtr()->log())
        {
            return;
        }
        if (!_logfile.is_open())
        {
            return;
        }

        //date curdate(day_clock::universal_day());
        //ptime curtime(microsec_clock::universal_time());
        time_t rawtime;
        tm* timeinfo;

        time(&rawtime);
        timeinfo = gmtime(&rawtime);
        char timeprefix[50];
        strftime(timeprefix, 50, "%x - %X: ", timeinfo);

        _logfile << timeprefix << text << std::endl;
    }

    static ErrorManager* getPtr()
    {
        static boost::shared_ptr<ErrorManager> _impl;
        if (!_impl.get())
        {
            _impl.reset(new ErrorManager);
        }
        return _impl.get();
    }
    ~ErrorManager()
    {
        _logfile.close();
    }
private:
    // Стандартный конструктор, читает из файла параметры
    ErrorManager()
    {
        if (!SettingsKeeper::getPtr()->log())
        {
            return;
        }
        _logfile.open("log.txt", std::ios::out | std::ios::trunc);
        if (!_logfile.is_open())
        {
            cout << "\nWarning: logfile 'log.txt' cannot be opened! Logging disabled!" << std::endl;
        }
    }
    ErrorManager(const ErrorManager& src);
    ErrorManager& operator=(const ErrorManager& rhs);

    std::ofstream _logfile;
};

#define sErrorMgr (*ErrorManager::getPtr())

#endif // ERRORMANAGER_H