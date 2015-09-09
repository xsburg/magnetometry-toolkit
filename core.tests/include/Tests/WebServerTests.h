#pragma once

#include <gtest/gtest.h>
#include "Utils/BaseTest.h"
#include <WebServer.h>
#include <Utils/Helpers.h>
#include <mongoose/mongoose.h>

using namespace Common;

namespace core
{
    namespace tests
    {
        class TestActionHandler : public WebServerActionHandler
        {
        public:
            QString name() const override
            {
                return "TestActionHandler";
            }

            bool match(mg_connection* connection) override
            {
                return this->exactMatch(connection, "api/test");
            }

            void execute(mg_connection* connection) override
            {
                mg_printf_data(connection, "{ \"result\": \"Some Test Data\" }");
            }
        };

        class WebServerTests : public BaseTest
        {
        };

        TEST_F(WebServerTests, ShouldStartServerAndAcceptARequest)
        {
            // Arrange
            auto server = std::make_shared<WebServer>();
            server->port(8000);
            server->addActionHandler(std::make_shared<TestActionHandler>());
            server->runAsync();

            // Act
            auto responseText = sHelpers.getResponse("http://localhost:8000/api/test");

            // Assert
            qDebug() << "200, data: " << responseText;
            ASSERT_TRUE(responseText == "{ \"result\": \"Some Test Data\" }");
        }
    }
}
