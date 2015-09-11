#pragma once

#include <vector>
#include <cmath>
#include <gtest/gtest.h>
#include <boost/thread.hpp>
#include "common/SmartPtr.h"
#include "common/Exception.h"
#include "common/Connection.h"

namespace core
{
    namespace tests
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
