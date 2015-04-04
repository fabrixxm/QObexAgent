#include <iostream>
#include "agentmanager1_interface.h"
#include "obexagent.h"
#include "utils.h"
#include <QApplication>
//#include <QCommandLineParser>
#include <QDBusConnection>
#include <QDBusInterface>


int main(int argc, char *argv[])
{
    bool r; //, test;

    QApplication app(argc, argv);
    QApplication::setQuitOnLastWindowClosed(false);
    QApplication::setApplicationName("QOobexAgent");
    QApplication::setApplicationVersion("1.0");

	/** QCommandLineParser is in Qt5 
    QCommandLineParser parser;
    parser.setApplicationDescription("Obex agent to ask user permission on bluetooth push.");
    parser.addHelpOption();
    parser.addVersionOption();

    QCommandLineOption testOption(QStringList() << "t" << "test", "Test icon");
    parser.addOption(testOption);
    parser.process(app);

    test = parser.isSet(testOption);
	*/
    // --

    ObexAgent *agent = new ObexAgent();

    // if (test) agent->test();

    // --
    QDBusConnection connection = QDBusConnection::sessionBus();

    org::bluez::obex::AgentManager1 agentManager("org.bluez.obex","/org/bluez/obex", connection);


//    std::cout << "Registering agent object " << AGENT_PATH << std::endl;
    r = connection.registerObject(AGENT_PATH, agent);
    if (!r) {
        std::cout << "Error registering agent object " << AGENT_PATH << std::endl;
        exit(-1);
    }

//    std::cout << "Registering agent service " << AGENT_SERVICE << std::endl;
    r = connection.registerService(AGENT_SERVICE);
    if (!r) {
        std::cout << "Error registering service " << AGENT_SERVICE  << std::endl;
        exit(-1);
    }


//    std::cout << "RegisterAgent" << AGENT_PATH << std::endl;
    QDBusPendingReply<void> ret;
    QDBusObjectPath agentObjectPath(AGENT_PATH);
    ret = agentManager.RegisterAgent(agentObjectPath);
    check_reply(&ret, -2);
//    std::cout << "Ok." << std::endl;


    return app.exec();
}
