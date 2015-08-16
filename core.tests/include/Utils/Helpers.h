#pragma once

#include <QtCore/QtCore>
#include "common/Singleton.h"
#include "common/SmartPtr.h"
#include "common/Exception.h"
#include "common/Connection.h"

using namespace Common;

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
            
        };
    }
}

#define sHelpers SINGLETON_INSTANCE(::core::tests::Helpers)