all: Serv Cli Light

Light: lightTest.cpp
	@echo "..compiling lightTest"
	@g++ lightTest.cpp -std=c++11 -lwiringPi -lwiringPiDev

Serv: server.cpp
	@echo "... compiling server"
	@g++ server.cpp -o Serv -std=c++11 -lPocoNet -lPocoData -lPocoDataSQLite -lPocoFoundation

Cli: client.cpp
	@echo "... comiling client.cpp"
	@g++ client.cpp -o Cli -std=c++11 -lPocoNet -lPocoData -lPocoDataSQLite -lPocoFoundation -lwiringPi -lwiringPiDev

clean: 
	rm Serv Cli
	rm *.o ouput
	rm a.out
