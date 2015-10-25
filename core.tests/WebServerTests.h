#pragma once

#include <gtest/gtest.h>
#include "BaseTest.h"
#include <WebServer.h>
#include <Helpers.h>
#include <mongoose/mongoose.h>

using namespace common;

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

            bool match() override
            {
                return this->exactMatch( "api/test");
            }

            void execute() override
            {
                mg_printf_data(connection(), "{ \"result\": \"Some Test Data\" }");
            }
        };

        class PostTestActionHandler : public WebServerActionHandler
        {
        public:
            QString name() const override
            {
                return "PostTestActionHandler";
            }

            bool match() override
            {
                return this->exactMatch( "api/test");
            }

            void execute() override
            {
                QString content(QByteArray(connection()->content, connection()->content_len));
                qDebug() << content;
                
                mg_printf_data(connection(), content.toLatin1());
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

        TEST_F(WebServerTests, ShouldStartServerAndAcceptPostRequest)
        {
            // Arrange
            auto server = std::make_shared<WebServer>();
            server->port(8001);
            server->addActionHandler(std::make_shared<PostTestActionHandler>());
            server->runAsync();

            // Act
            auto responseText = sHelpers.postResponse("http://localhost:8001/api/test", QString("{ \"command\": \"FIRE\" }"));

            // Assert
            qDebug() << "200, data: " << responseText;
            ASSERT_TRUE(responseText == "{ \"command\": \"FIRE\" }");
        }
    }
}
