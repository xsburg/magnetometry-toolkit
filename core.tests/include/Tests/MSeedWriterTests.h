#pragma once

#include <gtest/gtest.h>
#include "Utils/BaseTest.h"
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

        TEST_F(MSeedWriterTests, ShouldWriteAndReadIntegerMSeedRecord)
        {
            // Arrange
            QString fileName = this->ResolvePath("data.mseed");
            QVector<IntegerMSeedRecord::SharedPtr_t> data(4);
            for (int i = 0; i < 4; i++)
            {
                auto range = data[i] = std::make_shared<IntegerMSeedRecord>();
                range->channelName(QString("CH") + QString::number(i));
                range->location("MS");
                range->network("IF");
                range->station("IFZMK");
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
            auto records = reader->readAll();

            ASSERT_EQ(records.size(), 4);
            for (int i = 0; i < 4; i++)
            {
                auto record = records[i];
                ASSERT_TRUE(record->channelName() == data[i]->channelName());
                ASSERT_TRUE(record->location() == data[i]->location());
                ASSERT_TRUE(record->network() == data[i]->network());
                ASSERT_TRUE(record->station() == data[i]->station());
                ASSERT_TRUE(record->samplingRateHz() == data[i]->samplingRateHz());
                auto actualStartDate = record->startTime().toString(Qt::ISODate);
                auto expectedStartDate = data[i]->startTime().toString(Qt::ISODate);
                ASSERT_TRUE(actualStartDate == expectedStartDate);
                ASSERT_TRUE(record->getSampleType() == Integer);
                auto iRecord = std::dynamic_pointer_cast<IntegerMSeedRecord>(record);
                ASSERT_EQ(iRecord->data().size(), 500);
                ASSERT_TRUE(memcmp(iRecord->data().data(), data[i]->data().data(), data[i]->data().size() * 4) == 0);
            }
        }
    }
}
