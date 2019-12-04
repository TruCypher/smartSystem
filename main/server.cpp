#include "Poco/Net/DatagramSocket.h"
#include "Poco/Net/SocketAddress.h"
#include "Poco/Net/IPAddress.h"

#include "Poco/Data/SQLite/Connector.h"
#include "Poco/Data/Session.h"
#include "Poco/NamedTuple.h"
#include <iostream>
#include <iomanip>

using Poco::Data::Session;
using namespace Poco::Data::Keywords;
using std::setw;

int main(int argc, char** argv) {
    Poco::Data::SQLite::Connector::registerConnector();
    Poco::Net::SocketAddress sa("localhost", 8080);
    Poco::Net::DatagramSocket dgs(sa);

    Session session("SQLite", "./DB/devices.db");

    char buffer[1024];

    for(;;) {
       Poco::Net::SocketAddress sender;
       int n = dgs.receiveFrom(buffer, sizeof(buffer)-1, sender);
       buffer[n] = '\0';
       std::cout << sender.toString() <<  ":" << buffer << std::endl;

        typedef Poco::Tuple<int, std::string, std::string, std::string> DEV;
        std::vector<DEV> dev;
        try {
            session << buffer,into(dev), now;
            std::string res = "";
            for (DEV d : dev) {
                res += std::to_string(d.get<0>())
                + "|" + d.get<1>() + "|" + d.get<2>()
                + "|" + d.get<3>() + "\n";
            } 
            std::cout << res << std::endl;
            dgs.sendTo(res.data(), res.size(), sender);
        } catch (...) {
            std::string err = "SQL is not a valid statement";
            dgs.sendTo(err.data(), err.size(), sender);
        }
    }

    return 0;
}
