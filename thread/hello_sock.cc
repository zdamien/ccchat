// TCP server version of "hello world".  Largely typed in from UNP 3e,
// apart from using my own function wrappers.

#include "exc_socket.h"
#include <string>
#include <iostream>
#include <cstring>
#include <unistd.h>

const int SERV_PORT = 4000;

int main() {
    socket_t listenfd, connfd;
    socklen_t clilen;
    struct sockaddr_in cliaddr, servaddr;

    try {
        listenfd = Socket (AF_INET, SOCK_STREAM, 0);
        memset (&servaddr, 0, sizeof(servaddr));
        servaddr.sin_family = AF_INET;
        servaddr.sin_addr.s_addr = htonl (INADDR_ANY);
        servaddr.sin_port = htons (SERV_PORT);

        Bind (listenfd, (sockaddr *) &servaddr, sizeof(servaddr));
        Listen (listenfd, 20);

        while (true) {
            clilen = sizeof (cliaddr);
            connfd = Accept (listenfd, (sockaddr *) &cliaddr, &clilen);
            send (connfd, "hello", 6, 0);
            close (connfd);
        }
    }
    catch (const std::exception& e) {
        std::cout << "Exception: " << e.what() << "\n";
    }
}


