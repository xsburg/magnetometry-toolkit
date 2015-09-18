#include "Helpers.h"
#include <algorithm>
#include <gtest/gtest.h>
#include "common/File.h"

namespace core
{
    namespace tests
    {
        Helpers::Helpers()
        {
        }

        Helpers::~Helpers()
        {
        }

        void Helpers::assertBinaryArray(const QByteArray& expected, const QByteArray& actual)
        {
            int size = std::min(expected.size(), actual.size());
            bool bad = false;
            int badIndex = -1;
            for (int i = 0; i < size; ++i)
            {
                if (expected[i] != actual[i])
                {
                    sLogger.Info(QString("Bytes mismatch at %1.").arg(i));
                    bad = true;
                    badIndex = i;
                    break;
                }
            }
            if (bad)
            {
                auto expectedFile = File::CreateBinary("testData.expected.jps");
                expectedFile->write(expected);
                auto actualFile = File::CreateBinary("testData.actual.jps");
                actualFile->write(actual);
                ASSERT_EQ(expected[badIndex], actual[badIndex]);
            }
            ASSERT_EQ(expected.size(), actual.size());
        }

        QString Helpers::getResponse(const QString& url)
        {
            // create custom temporary event loop on stack
            QEventLoop eventLoop;

            // "quit()" the event-loop, when the network request "finished()"
            QNetworkAccessManager mgr;
            QObject::connect(&mgr, SIGNAL(finished(QNetworkReply*)), &eventLoop, SLOT(quit()));

            // the HTTP request
            QNetworkRequest req(url);
            QNetworkReply* reply;
            reply = mgr.get(req);

            eventLoop.exec(); // blocks stack until "finished()" has been called
            auto success = reply->error() == QNetworkReply::NoError;
            if (!success) {
                sLogger.Error("Failure: " + reply->errorString());
                throw new Exception(QString("Failed to performe web request to %1: %2").arg(url).arg(reply->errorString()));
            }
            return reply->readAll();
        }

        QString Helpers::postResponse(const QString& url, const QString& data)
        {
            // create custom temporary event loop on stack
            QEventLoop eventLoop;

            // "quit()" the event-loop, when the network request "finished()"
            QNetworkAccessManager mgr;
            QObject::connect(&mgr, SIGNAL(finished(QNetworkReply*)), &eventLoop, SLOT(quit()));

            // the HTTP request
            QNetworkRequest req(url);
            QNetworkReply* reply;
            reply = mgr.post(req, data.toLatin1());

            eventLoop.exec(); // blocks stack until "finished()" has been called
            auto success = reply->error() == QNetworkReply::NoError;
            if (!success) {
                sLogger.Error("Failure: " + reply->errorString());
                throw new Exception(QString("Failed to performe web request to %1: %2").arg(url).arg(reply->errorString()));
            }
            return reply->readAll();
        }
    }
}
