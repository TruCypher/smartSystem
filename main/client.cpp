#include "Poco/Net/DatagramSocket.h"
#include "Poco/Net/SocketAddress.h"
#include <wiringPi.h>

#include <iostream>

int main(int argc, char** argv) {
    int prevMode = 0;
    const int led = 7;
    wiringPiSetup();
    pinMode(led, INPUT);

    Poco::Net::SocketAddress sa("localhost", 8080);
    Poco::Net::DatagramSocket dgs;
    dgs.connect(sa);
    
    int user;
    char buffer[1024];

    printf("Choose (1) for admin or (0) for client: ");
    std::cin >> user;


    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    if (user == 1) {
        int exit = 0;
        while(true) {
            printf("enter a SQL command: ");
            std::string sql;
            std::getline(std::cin, sql);

            dgs.sendBytes(sql.data(), sql.size());
            int n = dgs.receiveBytes(buffer, sizeof(buffer)-1);
            buffer[n] = '\0';
            std::cout <<  buffer << std::endl;
        }
    } else if (user == 0) {
        std::string sql;
        while(true) {
            if(digitalRead(led) != prevMode) {
                if(digitalRead(led) == LOW) {
                    sql = "INSERT INTO devices (deviceName, deviceData) VALUES ('led', 'the led is turned off')";
                    dgs.sendBytes(sql.data(), sql.size());
                    int n = dgs.receiveBytes(buffer, sizeof(buffer)-1);
                    buffer[n] = '\0';
                    std::cout <<  buffer << std::endl;
                    prevMode = digitalRead(led);
                } else {
                    sql = "INSERT INTO devices (deviceName, deviceData) VALUES ('led', 'the led is turned on')";
                    dgs.sendBytes(sql.data(), sql.size());
                    int n = dgs.receiveBytes(buffer, sizeof(buffer)-1);
                    buffer[n] = '\0';
                    std::cout <<  buffer << std::endl;
                    prevMode = digitalRead(led);
                }
                delay(1500);
            }

        }

    } else {
        std::cout << "exit with code 404 NOT FOUND" << std::endl;
    }

    return 0;
}
