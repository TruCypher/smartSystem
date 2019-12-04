#include <stdio.h>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <signal.h>

void *get_in_addr(struct sockaddr *sa) {
    if (sa->sa_family == AF_INET) {
        return &(((struct sockaddr_in*)sa)->sin_addr);
    }

    return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

int main(int argc, char** argv) {
    const char* PORT = "8080";

    int socketfd;
    struct addrinfo hints, *servinfo, *p;
    int rv; 
    char s[INET6_ADDRSTRLEN];

    if(argc != 2) {
        printf("please enter hostname\n");
        return 1;
    }

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;

    if((rv = getaddrinfo(argv[1], PORT, &hints, &servinfo) != 0)) {
        printf("can't get adress info");
        return 1;
    }

    for(p=servinfo; p != NULL; p = p->ai_next) {
        if((socketfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1) {
            printf("error initializing socket \n");
            continue;
        }
        if(connect(socketfd, p->ai_addr, p->ai_addrlen) == -1) {
            close(socketfd);
            perror("client: socket");
            return 1;
        }

        break;
    }

    if (p==NULL) {
        printf("failed to connect");
        return 2;
    }

    inet_ntop(p->ai_family, get_in_addr((struct sockaddr *)p->ai_addr), s, sizeof(s));
    printf("client connecting to: %s\n", s);

    if(send(socketfd, "omg comomomomom", 100, 0) == -1) {
        perror("send: client");
        return 1;
    }

    return 0;
}