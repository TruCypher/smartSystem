#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>

int main(int argc, char* arg[]) {

    struct addrinfo hints, *p;
    int status;

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;

    if(status = getaddrinfo(arg[1],NULL, &hints, &p) != 0) {
        printf("can't get addr info\n");
        return 2;
    }

    if(arg[1] == NULL) {
        printf("put in host\n");
        return 1;
    }

    printf("arg is: %s\n", arg[1]);
    std::cout << p->ai_family << " " << p->ai_addr << " " << p->ai_socktype << std::endl;
    std::cout << AF_INET << " " << AF_INET6 << std::endl;
    freeaddrinfo(p);
    return 0;
}