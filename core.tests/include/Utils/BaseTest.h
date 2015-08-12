#pragma once

#include <vector>
#include <cmath>
#include <gtest/gtest.h>
#include <boost/thread.hpp>
#include "Common/SmartPtr.h"
#include "Common/Exception.h"
#include "Common/Connection.h"

namespace Greis
{
    namespace Tests
    {
        class BaseTest : public ::testing::Test
        {
        private:
            std::shared_ptr<Common::Connection> _connection;
        protected:
            BaseTest();

            virtual ~BaseTest();

            virtual void SetUp();

            virtual void TearDown();

            const std::shared_ptr<Common::Connection>& Connection() const;

            QString ResolvePath(const QString& fileName) const;

            QByteArray ReadJpsBinary(const QString& fileName) const;
        };
    }
}
