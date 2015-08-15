#include "Path.h"
#include <QtCore/QtCore>

namespace Common
{
    QString Path::ApplicationDirPath()
    {
        return QCoreApplication::applicationDirPath();
    }

    QString Path::ApplicationFilePath()
    {
        return QCoreApplication::applicationFilePath();
    }

    QString Path::Combine(const QString& a, const QString& b)
    {
        QString an = QDir::fromNativeSeparators(a);
        QString bn = QDir::fromNativeSeparators(b);
        if (!an.endsWith('/') && !bn.startsWith('/'))
        {
            an.append('/');
        }
        QString ret = QDir::toNativeSeparators(an + bn);
        return ret;
    }
}
