#pragma once

#include <vector>
#include <cmath>
#include <gtest/gtest.h>
#include <boost/thread.hpp>
#include "Utils/BaseTest.h"
#include "common/SmartPtr.h"
#include "common/Connection.h"

using namespace Common;

namespace core
{
    namespace tests
    {
        class EnvironmentTests : public BaseTest
        {
        };

        TEST_F(EnvironmentTests, ShouldConnect)
        {
            this->Connection()->DbHelper()->ExecuteQuery("SHOW TABLES;");
            sLogger.Info("Connection succeeded.");
        }
    }
}
