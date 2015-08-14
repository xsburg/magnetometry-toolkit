#pragma once

#include <vector>
#include <cmath>
#include <gtest/gtest.h>
#include <boost/thread.hpp>
#include "Utils/BaseTest.h"
#include "Common/SmartPtr.h"
#include <core/MSeedWriter.h>
#include <core/FileBinaryStream.h>
#include <core/MSeedReader.h>

using namespace Common;

namespace core
{
    namespace tests
    {
        class MSeedWriterTests : public BaseTest
        {
        };

        TEST_F(MSeedWriterTests, ShouldWriteIntegerSampleRange)
        {
            // Arrange
            QString fileName = this->ResolvePath("data.mseed");
            QVector<IntegerSampleRange::SharedPtr_t> data(4);
            for (int i = 0; i < 4; i++)
            {
                auto range = data[i] = std::make_shared<IntegerSampleRange>();
                range->channelName(QString("CH") + i);
                range->location("MSK");
                range->network("IFZN");
                range->station("IFZMSKM");
                range->samplingRateHz(3);
                range->startTime(QDateTime(QDate(2015, 6, 4), QTime(13, 44), Qt::UTC));
                for (int j = 0; j < 500; j++)
                {
                    range->data().push_back(j * (i + 1));
                }
            }

            // Act
            {
                auto stream = std::make_shared<FileBinaryStream>(fileName, true);
                auto writer = std::make_shared<MSeedWriter>(stream);
                for (auto& range : data)
                {
                    writer->write(range);
                }
                writer->close();
            }

            // Assert
            auto reader = std::make_shared<MSeedReader>(fileName);
            reader->read();
            /*for (auto e = file->Body().begin(); e != file->Body().end(); ++e)
            {
                for (auto it = (*e)->Messages.begin(); it != (*e)->Messages.end(); ++it)
                {
                    bool res = (*it)->Validate();
                    EXPECT_TRUE(res);
                }
            }

            auto ba = file->ToByteArray();
            auto outFile = File::CreateBinary("generatedJpsFile.jps");
            outFile->write(ba);
            outFile->close();*/
        }
    }
}
