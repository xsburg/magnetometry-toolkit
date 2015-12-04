#ifndef Exception_h__
#define Exception_h__

#include <QtCore/QtCore>

namespace common
{
    class Exception
    {
        QString _message;
    public:
        Exception(const QString& message)
        {
            _message = message;
        }

        Exception()
        {
        }

        virtual ~Exception()
        {
        }

        virtual QString what() const
        {
            return _message;
        }
    };
}

#endif // Exception_h__