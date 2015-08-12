#include "Utils/Helpers.h"
#include <algorithm>
#include <gtest/gtest.h>
#include "Common/File.h"

namespace Greis
{
    namespace Tests
    {
        Helpers::Helpers()
        {
        }

        Helpers::~Helpers()
        {
        }

        void Helpers::assertBinaryArray(const QByteArray& expected, const QByteArray& actual)
        {
            int size = std::min(expected.size(), actual.size());
            bool bad = false;
            int badIndex = -1;
            for (int i = 0; i < size; ++i)
            {
                if (expected[i] != actual[i])
                {
                    sLogger.Info(QString("Bytes mismatch at %1.").arg(i));
                    bad = true;
                    badIndex = i;
                    break;
                }
            }
            if (bad)
            {
                auto expectedFile = File::CreateBinary("testData.expected.jps");
                expectedFile->write(expected);
                auto actualFile = File::CreateBinary("testData.actual.jps");
                actualFile->write(actual);
                ASSERT_EQ(expected[badIndex], actual[badIndex]);
            }
            ASSERT_EQ(expected.size(), actual.size());
        }
    }
}
