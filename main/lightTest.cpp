#include <iostream>
extern "C" {
#include <wiringPi.h>
}


int main () {
   const int led = 7;

   wiringPiSetup();

   pinMode(led, INPUT);

   while(true) {
       if (digitalRead(led) == LOW) {
           std::cout << digitalRead(led) << " low:" << LOW << " high:" << HIGH << std::endl;
       } else {
           std::cout << digitalRead(led) << " low:" << LOW << " high:" << HIGH  << std::endl;
       }
        delay(1000);
   }

   return 0;
}

