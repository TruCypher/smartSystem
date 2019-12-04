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

int main() {
    char buf[100];
    const char* PORT = "8080";
    const int BACKLOG = 10;

    ssize_t numBytes;
    int socketfd, clfd, len, newfd;
    struct addrinfo hints, *servinfo, *p;
    struct sockaddr_storage their_addr;
    socklen_t sin_size;
    struct sigaction sa;
    int yes = 1;
    char s[INET6_ADDRSTRLEN];
    int rv;

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;

    if((rv = getaddrinfo(NULL, PORT, &hints, &servinfo)) != 0) {
        printf("can't get the addr_info");
        return 1;
    }

    //looping through result and bind
    for(p=servinfo; p != NULL; p = p->ai_next) {
        if((socketfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1) {
            perror("server: socket");
            continue;
        }
        if(setsockopt(socketfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1) {
            perror("setsocketopt");
            return 1;
        }
        if(bind(socketfd, p->ai_addr, p->ai_addrlen) == -1) {
            close(socketfd);
            perror("server: bind");
            continue;
        }
        break;
    }
    freeaddrinfo(servinfo);

    if(p == NULL) {
        printf("server is failed to bind");
    }
    if(listen(socketfd, BACKLOG) == -1) {
        perror("listen");
        return 1;
    }

    printf("pending for connnection: \n");

    while(true) {
        int in_size = sizeof their_addr;
        newfd = accept(socketfd, (struct sockaddr *)&their_addr, &sin_size);
        if (newfd == -1) {
            perror("accept");
            continue;
        }

        inet_ntop(their_addr.ss_family,
                get_in_addr((struct sockaddr *)&their_addr),
                s, 
                sizeof s);
        printf("server: got connection from %s\n", s);

        char buf[4096];
        while(1) {
            memset(buf, 0, 4096);
            int byterecv = recv(newfd, buf, 4096, 0);
            if(byterecv == -1) {
                perror("server: recv");
                break;
            }

            if(byterecv == 0) {
                printf("user disconnected");
                break;
            }

            printf("client send: %s\n", buf);
        }

        close(newfd);
    }

    //close socket
    return 0; 
}