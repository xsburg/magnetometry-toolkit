#pragma once

#include <QtCore/QtCore>
#include "Common/Singleton.h"
#include "Common/SmartPtr.h"
#include "Common/Exception.h"
#include "Common/Connection.h"

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