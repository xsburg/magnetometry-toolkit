#ifndef Path_h__
#define Path_h__

#include <QtCore/QString>

namespace Common
{
    class Path
    {
    public:
        static QString ApplicationDirPath();
        static QString ApplicationFilePath();
        static QString Combine(const QString& a, const QString& b);
    };
}

#endif // Path_h__