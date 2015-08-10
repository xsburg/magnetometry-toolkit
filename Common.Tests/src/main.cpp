#include <gtest/gtest.h>
#include "BitConverterTests.h"
#include "Common/Connection.h"

#include <QtCore>

int main(int argc, char **argv)
{
    QString test = "sad";
    Common::Connection c;
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
