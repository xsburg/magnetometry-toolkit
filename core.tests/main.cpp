#include <clocale>
#include <locale>
#include <QtCore/QtCore>
#include <gtest/gtest.h>
#include "common/Logger.h"
#include "common/Path.h"

#include "tests.h"

using namespace Common;

int main(int argc, char **argv)
{
#ifdef WIN32
    std::setlocale(LC_ALL, "en_US.utf-8");
    std::locale::global(std::locale("en-US"));
#else
    std::setlocale(LC_ALL, "en_US.UTF-8");
    std::locale::global(std::locale("en_US.UTF-8"));
#endif

    QCoreApplication a(argc, argv);

    QTextCodec* codec = QTextCodec::codecForName("UTF-8");
    QTextCodec::setCodecForLocale(codec);

    //sLogger.Initialize(Path::Combine(Path::ApplicationDirPath(), "logger.config.xml"));
    sLogger.Initialize(LogLevel::Debug);
    sIniSettings.Initialize(Path::Combine(Path::ApplicationDirPath(), "config.test.ini"));
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
