#ifndef JpsFile_h__
#define JpsFile_h__

#include "GreisMessageStream.h"
#include <vector>
#include <boost/utility.hpp>
#include "Epoch.h"
#include "Common/SmartPtr.h"
#include "StdMessage/RcvTimeStdMessage.h"
#include "StdMessage/RcvDateStdMessage.h"
#include <QtCore/QtCore>

namespace Greis
{
    class DataChunk : private boost::noncopyable
    {
    public:
        SMART_PTR_T(DataChunk);

        static DataChunk::UniquePtr_t FromFile(QString filename, bool skipInvalid = false);

        inline const std::vector<Message::UniquePtr_t>& Head() const { return _head; }
        inline std::vector<Message::UniquePtr_t>& Head() { return _head; }
        inline const std::vector<Epoch::UniquePtr_t>& Body() const { return _body; }
        inline std::vector<Epoch::UniquePtr_t>& Body() { return _body; }

        QByteArray ToByteArray() const;

        void AddMessage(Message::UniquePtr_t msg);

        DataChunk();

        inline Epoch::UniquePtr_t& UnfinishedEpoch()
        {
            return _lastEpoch;
        }
    private:

        inline void updateTimePart( RcvTimeStdMessage* msg )
        {
            _dateTime.setTime(QTime(0, 0).addMSecs(msg->Tod()));
            _timeIsSet = true;
        }

        inline void updateDatePart( RcvDateStdMessage* msg ) 
        {
            _dateTime.setDate(QDate(msg->Year(), msg->Month(), msg->Day()));
            _dateIsSet = true;
        }

        inline bool dateTimeIsSet() const { return _dateIsSet && _timeIsSet; }

        std::vector<Message::UniquePtr_t> _head;
        std::vector<Epoch::UniquePtr_t> _body;
        Epoch::UniquePtr_t _lastEpoch;
        QDateTime _dateTime;
        int _epochCounter;
        bool _dateIsSet;
        bool _timeIsSet;
    };
}

#endif // JpsFile_h__