#include <iostream>
#include <vector>

#include "Poco/Data/SQLite/Connector.h"
#include "Poco/Data/Session.h"
#include "Poco/NamedTuple.h"


using namespace Poco::Data::Keywords;
using Poco::Data::Session;
using Poco::Data::Statement;

class Device {
    std::string name;
    std::string data;
};

int main() {

    //Open SQLite Connector
    Poco::Data::SQLite::Connector::registerConnector();
    //create session
    Session session("SQLite","./DB/devices.db");

    
    //SQLITE QUERY
    session << "CREATE TABLE IF NOT EXISTS devices"
                "(deviceId INTEGER PRIMARY KEY," 
                "deviceName VARCHAR(40),"
                "deviceData VARCHAR(40),"
                "deviceDate DATE DEFAULT CURRENT_TIMESTAMP);", now;

    //std::vector<std::string> name = {"smartwatch", "smartfrige", "computer", "PI"};
    //std::vector<std::string> data = {"it's hot", "it's cold", "it's warm", "it's broken"};

    //for(int i=0;i<40;i++) {
    //    int k = rand() % 4;
    //    int w = rand() % 4;
    //    session << "INSERT INTO devices (deviceName, deviceData) VALUES(?,?)",
    //        use(name[k]),
    //        use(data[w]),
    //        now;
    //}
    
    typedef Poco::Tuple<int, std::string, std::string, std::string> DEV;
    std::vector<DEV> dev;
    session << "SELECT * FROM DEVICES", into(dev), now;
    for (DEV d : dev) {
        std::cout << d.get<0>() << " | " 
        << d.get<1>() << " | " 
        << d.get<2>() << " | " 
        << d.get<3>() << std::endl;
    }

    Poco::Data::SQLite::Connector::unregisterConnector();
    return 0;
}
