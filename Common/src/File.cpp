#include "File.h"
#include "FileException.h"

namespace Common
{
    QFilePtr File::OpenReadBinary(QString filename)
    {
        QFilePtr file(new QFile(filename));
        if (!file->open(QIODevice::ReadOnly))
        {
            throw FileException(QString("Ошибка при открытии файла '%1': %2.").arg(filename).arg(file->errorString()));
        }
        return file;
    }

    QFilePtr File::CreateBinary(QString filename)
    {
        QFilePtr file(new QFile(filename));
        if (!file->open(QIODevice::WriteOnly | QIODevice::Truncate))
        {
            throw FileException(QString("Ошибка при открытии файла '%1': %2.").arg(filename).arg(file->errorString()));
        }
        return file;
    }
}
