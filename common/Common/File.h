#ifndef File_h__
#define File_h__

#include <memory>
#include <QtCore/QFile>
#include <QtCore/QString>

namespace Common
{
    typedef std::shared_ptr<QFile> QFilePtr;

    class File
    {
    public:
        // Открытие файла на чтение в бинарном режиме. Ошибка если файла нет.
        static QFilePtr OpenReadBinary(QString filename);

        // Создание файла или удаление содержимого имеющегося файла и его открытие. В бинарном режиме.
        static QFilePtr CreateBinary(QString filename);
    };
}

#endif // File_h__
