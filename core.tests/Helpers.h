#pragma once

#include <QtCore/QtCore>
#include "common/Singleton.h"

using namespace common;

namespace core
{
    namespace tests
    {
        class Helpers
        {
            SINGLETON_BLOCK(Helpers)
        public:
            Helpers();
            ~Helpers();

            void assertBinaryArray(const QByteArray& expected, const QByteArray& actual);
            
            QString getResponse(const QString& url);
            QString postResponse(const QString& url, const QString& data);
        };
    }
}

#define sHelpers SINGLETON_INSTANCE(::core::tests::Helpers)