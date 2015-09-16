#include <QtCore/QtCore>
#include "common/Logger.h"
#include "common/SmartPtr.h"
#include "common/Path.h"
#include "common/Connection.h"
#include <boost/system/system_error.hpp>
#include <EbDeviceManager.h>

using namespace Common;
/*
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "mongoose.h"

static const char *s_no_cache_header =
"Cache-Control: max-age=0, post-check=0, "
"pre-check=0, no-store, no-cache, must-revalidate\r\n";

static void handle_restful_call(struct mg_connection *conn) {
    char n1[100], n2[100];

    // Get form variables
    mg_get_var(conn, "n1", n1, sizeof(n1));
    mg_get_var(conn, "n2", n2, sizeof(n2));

    mg_printf_data(conn, "{ \"result\": %lf }", strtod(n1, NULL) + strtod(n2, NULL));
}

static int ev_handler(struct mg_connection *conn, enum mg_event ev) {
    switch (ev) {
    case MG_AUTH: return MG_TRUE;
    case MG_REQUEST:
        if (!strcmp(conn->uri, "/api/sum")) {
            handle_restful_call(conn);
            return MG_TRUE;
        }
        mg_send_file(conn, "index.html", s_no_cache_header);
        return MG_MORE;
    default: return MG_FALSE;
    }
}*/

int main(int argc, char** argv)
{
    try
    {
        QCoreApplication a(argc, argv);

        QTextCodec* codec = QTextCodec::codecForName("UTF-8");
        QTextCodec::setCodecForLocale(codec);

        sIniSettings.Initialize(Path::Combine(Path::ApplicationDirPath(), "config.ini"));
        sLogger.Initialize(sIniSettings.value("LogLevel", 5).toInt());

        sLogger.Debug("The following sqldrivers are available:");
        auto sqlDrivers = QSqlDatabase::drivers();
        for (auto it = sqlDrivers.begin(); it != sqlDrivers.end(); ++it)
        {
            QString sqlDriverName = *it;
            sLogger.Debug(sqlDriverName);
        }


        auto device = std::make_shared<core::EbDeviceManager>();

        device->connect("/dev/ttyUSB0");






        return 0;
    }
    catch (boost::system::system_error& bex)
    {
        if (bex.code().value() == 2) {
            sLogger.Error("Connection to receiver could not be made.");
            sLogger.Error("The application could not find the port specified.");
            sLogger.Error("It is ether wrong configuration or the receiver moved to another port.");
        } else {
            sLogger.Error("No connection to receiver, code: " + QString::number(bex.code().value()));
        }
        return 1;
    }
    catch (Exception& e)
    {
        sLogger.Error(e.what());
        return 1;
    }
}
