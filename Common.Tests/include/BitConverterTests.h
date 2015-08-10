#ifndef BitConverterTests_h__
#define BitConverterTests_h__

#include "gtest/gtest.h"
#include "Common/BitConverter.h"

using namespace Common;

namespace
{
    class BitConverterTests : public ::testing::Test
    {
    protected:
        BitConverterTests() 
            : _lsbBc(BitConverter::LeastSignificantByte)
            , _msbBc(BitConverter::MostSignificantByte)
        {
        }

        virtual ~BitConverterTests()
        {
        }

        virtual void SetUp()
        {
        }

        virtual void TearDown()
        {
        }

        BitConverter _lsbBc;
        BitConverter _msbBc;
    };

    void testGetMethods(const BitConverter& bc);
    void testToMethods(const BitConverter& bc);

    TEST_F(BitConverterTests, LeastSignificantByteMode_GetMethods)
    {
        testGetMethods(_lsbBc);
    }

    /*TEST_F(BitConverterTests, MostSignificantByteMode_GetMethods)
    {
        testGetMethods(_msbBc);
    }*/

    TEST_F(BitConverterTests, LeastSignificantByteMode_ToMethods)
    {
        testToMethods(_lsbBc);
    }

    /*TEST_F(BitConverterTests, MostSignificantByteMode_ToMethods)
    {
        testToMethods(_msbBc);
    }*/

    void testGetMethods(const BitConverter& bc)
    {
        // unsigned
        {
            unsigned char expected = 234;
            auto actual = bc.GetUChar((const char*) &expected);
            EXPECT_EQ(expected, actual);
        }
        {
            unsigned short expected = 12345;
            auto actual = bc.GetUShort((const char*) &expected);
            EXPECT_EQ(expected, actual);
        }
        {
            unsigned int expected = 1234567891;
            auto actual = bc.GetUInt((const char*) &expected);
            EXPECT_EQ(expected, actual);
        }
        {
            unsigned long long expected = 12345678912345;
            auto actual = bc.GetULongLong((const char*) &expected);
            EXPECT_EQ(expected, actual);
        }
        // signed
        {
            char expected = -112;
            auto actual = bc.GetChar((const char*) &expected);
            EXPECT_EQ(expected, actual);
        }
        {
            short expected = -12345;
            auto actual = bc.GetShort((const char*) &expected);
            EXPECT_EQ(expected, actual);
        }
        {
            int expected = -1234567891;
            auto actual = bc.GetInt((const char*) &expected);
            EXPECT_EQ(expected, actual);
        }
        {
            long long expected = -12345678912345;
            auto actual = bc.GetLongLong((const char*) &expected);
            EXPECT_EQ(expected, actual);
        }
        // floating point
        {
            float expected = 12345.67891f;
            auto actual = bc.GetFloat((const char*) &expected);
            EXPECT_EQ(expected, actual);
        }
        {
            double expected = 1234512345.67891234;
            auto actual = bc.GetDouble((const char*) &expected);
            EXPECT_EQ(expected, actual);
        }
    }

    void testToMethods(const BitConverter& bc)
    {
        // unsigned
        {
            unsigned char expected = 234;
            char buffer[sizeof(expected)];
            bc.ToByteArray(expected, buffer);
            EXPECT_EQ(expected, *((decltype(expected)*)buffer));
        }
        {
            unsigned short expected = 12345;
            char buffer[sizeof(expected)];
            bc.ToByteArray(expected, buffer);
            EXPECT_EQ(expected, *((decltype(expected)*)buffer));
        }
        {
            unsigned int expected = 1234567891;
            char buffer[sizeof(expected)];
            bc.ToByteArray(expected, buffer);
            EXPECT_EQ(expected, *((decltype(expected)*)buffer));
        }
        {
            unsigned long long expected = 12345678912345;
            char buffer[sizeof(expected)];
            bc.ToByteArray(expected, buffer);
            EXPECT_EQ(expected, *((decltype(expected)*)buffer));
        }
        // signed
        {
            char expected = -112;
            char buffer[sizeof(expected)];
            bc.ToByteArray(expected, buffer);
            EXPECT_EQ(expected, *((decltype(expected)*)buffer));
        }
        {
            short expected = -12345;
            char buffer[sizeof(expected)];
            bc.ToByteArray(expected, buffer);
            EXPECT_EQ(expected, *((decltype(expected)*)buffer));
        }
        {
            int expected = -1234567891;
            char buffer[sizeof(expected)];
            bc.ToByteArray(expected, buffer);
            EXPECT_EQ(expected, *((decltype(expected)*)buffer));
        }
        {
            long long expected = -12345678912345;
            char buffer[sizeof(expected)];
            bc.ToByteArray(expected, buffer);
            EXPECT_EQ(expected, *((decltype(expected)*)buffer));
        }
        // floating point
        {
            float expected = 12345.67891f;
            char buffer[sizeof(expected)];
            bc.ToByteArray(expected, buffer);
            EXPECT_EQ(expected, *((decltype(expected)*)buffer));
        }
        {
            double expected = 1234512345.67891234;
            char buffer[sizeof(expected)];
            bc.ToByteArray(expected, buffer);
            EXPECT_EQ(expected, *((decltype(expected)*)buffer));
        }
    }
}

#endif // BitConverterTests_h__