
#include <boost/filesystem.hpp>

#include <QApplication>
#include <QQmlApplicationEngine>
#include <QQmlEngine>
#include <QtQml>
#include <QStandardPaths>
#include <qqmlengine.h>
#include <qqmlcontext.h>

#include "log/WarLog.h"

#include "darkspeak/darkspeak.h"
#include "darkspeak-gui.h"
#include "darkspeak/ImManager.h"

#include "DarkRoot.h"
#include "ContactsModel.h"
#include "ChatMessagesModel.h"
#include "ContactData.h"

using namespace darkspeak;
using namespace war;
using namespace std;

int main(int argc, char *argv[])
{
    // Let QT eat it's command-line arguments
    QApplication app(argc, argv);

    // Prepare our own stuff
    log::LogEngine logger;

    logger.AddHandler(make_shared<war::log::LogToStream>(
        clog, "console", log::LL_DEBUG));

    LOG_DEBUG << "Starting up with cwd='"
        << boost::filesystem::current_path().string()
        << "'";

    boost::filesystem::path conf_path = QStandardPaths::locate(
                QStandardPaths::AppDataLocation,
                {},
                QStandardPaths::LocateDirectory).toUtf8().constData();

    if (conf_path.empty()) {
        conf_path = QStandardPaths::locate(
                    QStandardPaths::HomeLocation,
                    {},
                    QStandardPaths::LocateDirectory).toUtf8().constData();

        conf_path /= ".darkspeak";
    }

    if (!boost::filesystem::is_directory(conf_path)) {
        LOG_NOTICE << "Creating data-dir: " << log::Esc(conf_path.string());
        boost::filesystem::create_directory(conf_path);
    }

    boost::filesystem::current_path(conf_path);

    logger.AddHandler(make_shared<war::log::LogToFile>(
        "darkspeak.log", true, "file", log::LL_TRACE4));

    LOG_INFO << "Using "
        << log::Esc(boost::filesystem::current_path().string())
        << " as data-dir and current dir.";

    conf_path /= "darkspeak.conf";

    // TODO: Create configuration file if it don't exist.
    //      Pop up a configuration dialog when the Gui starts.

    auto manager = impl::ImManager::CreateInstance(conf_path);
    DarkRoot dark_root(*manager);
    ContactsModel contacts_model(*manager);

    {
        darkspeak::Api::Info& info = dark_root.GetInfo();
        info.id = manager->GetConfigValue("service.dark_id");
    }

    // Initiallze the UI components
    QQmlApplicationEngine engine;

    {
        QString no_create_message = "ContactsModel is a global signleton.";
        qmlRegisterUncreatableType<ContactsModel>("com.jgaa.darkspeak", 1, 0, "ContactsModel", no_create_message);
    }

    {
        QString no_create_message = "ChatMessagesModel is obtained from ContactsModel.";
        qmlRegisterUncreatableType<ChatMessagesModel>("com.jgaa.darkspeak", 1, 0, "ChatMessagesModel", no_create_message);
    }

    qRegisterMetaType<darkspeak::Api::Message::ptr_t>("darkspeak::Api::Message::ptr_t");
    qRegisterMetaType<std::string>("std::string");

    //{
    //    QString no_create_message = "ContactData is obtained from ContactsModel.";
    //    qmlRegisterUncreatableType<ContactData>("com.jgaa.darkspeak", 1, 0, "ContactData", no_create_message);
    //}

    qmlRegisterType<ContactData>("com.jgaa.darkspeak", 1, 0, "ContactData");

    //qmlRegisterType<ContactsModel>("com.jgaa.darkspeak", 1, 0, "ContactsModel");
    //FoldersModel folders_model(*client);

    engine.rootContext()->setContextProperty("darkRoot", &dark_root);
    engine.rootContext()->setContextProperty("contactsModel", &contacts_model);
    engine.load(QUrl("qrc:/qml/main.qml"));
    //engine.rootContext()->setContextProperty("darkRoot", &dark_root);

    auto rval = app.exec();

    manager->Disconnect();

    // TODO: Wait for the threadpool to finish
    manager.reset();

    return rval;
}

