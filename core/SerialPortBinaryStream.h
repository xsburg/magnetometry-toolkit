#pragma once

#include <boost/asio.hpp>
#include <vector>
#include <iostream>
#include <QtCore/QtCore>

#ifndef Q_OS_WIN
#include <termio.h>
#include <fcntl.h>
#include <err.h>
#include <linux/serial.h>

static int rate_to_constant(int baudrate) {
#define B(x) case x: return B##x
    switch(baudrate) {
        B(50);     B(75);     B(110);    B(134);    B(150);
        B(200);    B(300);    B(600);    B(1200);   B(1800);
        B(2400);   B(4800);   B(9600);   B(19200);  B(38400);
        B(57600);  B(115200); B(230400); B(460800); B(500000); 
        B(576000); B(921600); B(1000000);B(1152000);B(1500000); 
    default: return 0;
    }
#undef B
}
#endif

#include "IBinaryStream.h"
#include "common/Exception.h"

namespace core
{
    class SerialPortBinaryStream : public IBinaryStream
    {
    private:
        boost::asio::io_service _io;
        boost::asio::serial_port _serial;
        std::string _portName;

        QByteArray _peekedData;
    public:
        SMART_PTR_T(SerialPortBinaryStream);
        
        /**
         * Constructor.
         * \param portName device name, example "/dev/ttyUSB0" or "COM4"
         * \param baudRate communication speed, example 9600 or 115200
         * \throws boost::system::system_error if cannot open the
         * serial device
         */
        SerialPortBinaryStream(std::string portName, unsigned int baudRate) : _io(), _serial(_io, portName), _portName(portName)
        {
#ifdef Q_OS_WIN
            _serial.set_option(boost::asio::serial_port_base::baud_rate(baudRate));
#else
            auto fd = _serial.native();
            struct serial_struct serinfo;
            struct termios options;

            tcgetattr(fd, &options);
            cfsetispeed(&options, rate_to_constant(baudRate));
            cfsetospeed(&options, rate_to_constant(baudRate));
            if (tcsetattr(fd, TCSANOW, &options) != 0) {
                std::cerr << "Failed to set attr";
                return;
            }
#endif
            _serial.set_option(boost::asio::serial_port_base::flow_control(boost::asio::serial_port_base::flow_control::none));
            _serial.set_option(boost::asio::serial_port_base::parity(boost::asio::serial_port_base::parity::none));
            _serial.set_option(boost::asio::serial_port_base::stop_bits(boost::asio::serial_port_base::stop_bits::one));
            _serial.set_option(boost::asio::serial_port_base::character_size(8));
        }

        boost::asio::serial_port& serialPort() { return _serial; }

        void write(QByteArray data)
        {
            boost::asio::write(_serial, boost::asio::buffer(data.data(), data.size()));
        }

        void write(const char* data, qint64 len)
        {
            boost::asio::write(_serial, boost::asio::buffer(data, len));
        }

        QByteArray read(qint64 maxlen)
        {
            QByteArray data(maxlen, 0);
            int actualRead = read(data.data(), data.size());
            data.resize(actualRead);
            return data;
        }

        QByteArray peek(qint64 maxlen)
        {
            QByteArray buf(maxlen, 0);
            int read = peek(buf.data(), maxlen);
            buf.resize(read);
            return buf;
        }
        
        qint64 read(char * data, qint64 maxSize)
        {
            qint64 read = 0;
            if (_peekedData.size() > maxSize)
            {
                int dataToRead = maxSize;
                memcpy(data, _peekedData.data(), dataToRead);
                int newBufLength = _peekedData.size() - dataToRead;
                memmove(_peekedData.data(), _peekedData.data() + dataToRead, newBufLength);
                _peekedData.resize(newBufLength);
                return dataToRead;
            } else if (_peekedData.size() > 0)
            {
                memcpy(data, _peekedData.data(), _peekedData.size());
                read = _peekedData.size();
                maxSize -= read;
                data += read;
                _peekedData.clear();
            }

            if (maxSize == 0)
            {
                return read;
            }

            read += boost::asio::read(_serial, boost::asio::buffer(data, maxSize));
            return read;
        }

        qint64 peek(char * data, qint64 maxSize)
        {
            if (_peekedData.size() >= maxSize)
            {
                memcpy(data, _peekedData.data(), maxSize);
                return maxSize;
            } else if (_peekedData.size() > 0)
            {
                memcpy(data, _peekedData.data(), _peekedData.size());
                data += _peekedData.size();
                maxSize -= _peekedData.size();
                qint64 read = _peekedData.size();
                QByteArray buf(maxSize, 0);
                read += boost::asio::read(_serial, boost::asio::buffer(buf.data(), buf.size()));
                memcpy(data, buf.data(), read - _peekedData.size());
                _peekedData.append(buf);
                return read;
            }

            _peekedData.resize(maxSize);
            qint64 read = boost::asio::read(_serial, boost::asio::buffer(_peekedData.data(), _peekedData.size()));
            memcpy(data, _peekedData.data(), read);
            return read;
        }

        bool isOpen()
        {
            return _serial.is_open();
        }

        void close() 
        {
            _serial.close();
        }

        void open()
        {
            _serial.open(_portName);
        }
    };
}
