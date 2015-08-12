#ifndef MSEEDSETTINGS_H
#define MSEEDSETTINGS_H

#include <boost/program_options.hpp>
#include <boost/shared_ptr.hpp>
#include <ctime>
#include <fstream>
#include <string>
#include <cassert>
#include <iostream>
#include <vector>

using std::string;
using std::vector;
using std::cout;

namespace po = boost::program_options;

//#define DATE_LIMIT

class SettingsKeeper
{
public:
    string location() const
    {
        return _vm["mseed.location"].as<string>();
    }
    string network() const
    {
        return _vm["mseed.network"].as<string>();
    }
    string station() const
    {
        return _vm["mseed.station"].as<string>();
    }
    string filename() const
    {
        return _vm["mseed.filename"].as<string>();
    }
    vector<string> channelNames() const
    {
        vector<string> names;
        names.push_back(_vm["mseed.channel1"].as<string>());
        names.push_back(_vm["mseed.channel2"].as<string>());
        names.push_back(_vm["mseed.channel3"].as<string>());
        names.push_back(_vm["mseed.channel4"].as<string>());
        names.push_back(_vm["mseed.channel5"].as<string>());
        names.push_back(_vm["mseed.channel6"].as<string>());
        return names;
    }
    // 0 = single, 1 = multi
    int writeType() const
    {
        return _vm["mseed.writeType"].as<int>();
    }

    bool log() const
    {
        if (_vm["mseed.log"].as<string>() == "true")
        {
            return true;
        } else {
            return false;
        }
    }

    static SettingsKeeper* getPtr()
    {
        static boost::shared_ptr<SettingsKeeper> _impl;
        if (!_impl.get())
        {
            _impl.reset(new SettingsKeeper);
        }
        return _impl.get();
    }
private:
    // Стандартный конструктор, читает из файла параметры
    SettingsKeeper()
    {
#ifdef DATE_LIMIT
        time_t ct = time(0);
        tm* curDate = localtime(&ct);
        if (curDate->tm_year != 109 || (curDate->tm_mon > 8 || curDate->tm_mon < 7)) // до сентября включительно
        {
            cout << "Test period has expired! Pay the money!!11" << std::endl;
            exit(1);
        }
#endif
        try {
            po::options_description config("Configuration");
            config.add_options()
                ("mseed.filename", po::value<string>(), "template for MSeed files name")
                ("mseed.writeType", po::value<int>(), "Type of writing MSeed data. 0 = write single file, 1 = write many files.")
                ("mseed.log", po::value<string>(), "True or False")
                ("mseed.network", po::value<string>(), "MSeed network")
                ("mseed.station", po::value<string>(), "MSeed station")
                ("mseed.location", po::value<string>(), "MSeed location")
                ("mseed.channel1", po::value<string>(), "MSeed channel 1 name")
                ("mseed.channel2", po::value<string>(), "MSeed channel 2 name")
                ("mseed.channel3", po::value<string>(), "MSeed channel 3 name")
                ("mseed.channel4", po::value<string>(), "MSeed channel 4 name")
                ("mseed.channel5", po::value<string>(), "MSeed channel 5 name")
                ("mseed.channel6", po::value<string>(), "MSeed channel 6 name")
                ;

            std::ifstream file("config.ini");
            if (!file.is_open())
            {
                cout << "File is not open error";
			    assert(false);
            }
            store(parse_config_file(file, config), _vm);
            notify(_vm);
        }
        catch(po::error& e)
        {
            cout << "Boost::program_options exception caught while reading config.ini: " << e.what() << std::endl;
            assert(false);
        }
        catch(std::exception& e)
        {
            cout << "Std::exception caught while reading config.ini: " << e.what() << std::endl;
            assert(false);
        }
    }
    SettingsKeeper(const SettingsKeeper& src);
    SettingsKeeper& operator=(const SettingsKeeper& rhs);

    po::variables_map _vm;
};

#endif // MSEEDSETTINGS_H