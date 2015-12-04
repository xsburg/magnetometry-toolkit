// ***********************************************************************
// <author>Stephan Burguchev</author>
// <copyright company="Stephan Burguchev">
//   Copyright (c) Stephan Burguchev 2012-2015. All rights reserved.
// </copyright>
// <summary>
//   JsonTests.h
// </summary>
// ***********************************************************************
#pragma once

#include <gtest/gtest.h>
#include "BaseTest.h"

using namespace common;

namespace core
{
    namespace tests
    {
        class JsonTests : public BaseTest
        {
        };

        TEST_F(JsonTests, ShouldSerializeObject)
        {
            // Arrange
            QJsonDocument doc = QJsonDocument::fromJson("{ \"command\": \"RUN\", \"interval\": 42.2, \"active\": true }");

            // Act
            auto root = doc.object();
            auto command = root.value("command").toString();
            auto interval = root.value("interval").toDouble();
            auto active = root.value("active").toBool();

            // Assert
            ASSERT_TRUE(command == "RUN");
            ASSERT_TRUE(qFuzzyCompare(interval, 42.2));
            ASSERT_TRUE(active == true);
        }
    }
}