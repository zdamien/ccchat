// TCP echo server.  Largely typed in from UNP 3e,
// apart from using my own function wrappers.

#include "exc_socket.h"
#include "unp_brief.h" // for Readline

#include <string>
#include <iostream>
#include <cstring>
#include <unistd.h>

//#include <netdb.h>

const int SERV_PORT = 4000;

void str_echo (socket_t sockfd) {
    ssize_t n;
    char buf[MAXLINE];

    while ( (n = recv (sockfd, buf, MAXLINE, 0)) > 0) {
        send(sockfd, buf, n, 0);
    }
}

void str_cli (FILE *fp, socket_t sockfd) {
    char from_user[MAXLINE], from_socket[MAXLINE];

    while (fgets (from_user, MAXLINE, fp) != nullptr) {
        send (sockfd, from_user, strlen(from_user), 0);
        
        int nread = Readline (sockfd, from_socket, MAXLINE);
        if (nread <= 0) return;
        fputs (from_socket, stdout);
    }
}
int main(int argc, char** argv) {
    socket_t connfd;
    socklen_t clilen;
    pid_t child_pid;
    int port = SERV_PORT;

    if (argc == 3)
        port = atoi (argv[2]);

    try {

        /*
        connfd = Socket (AF_INET, SOCK_STREAM, 0);
        sockaddr_in servaddr;
        memset (&servaddr, 0, sizeof(servaddr));
        servaddr.sin_family = AF_INET;
        servaddr.sin_port = htons (port);
        int rv = inet_pton (AF_INET, argv[1], &servaddr.sin_addr);
        {
            char buf[4096];
            printf("rv %d %s\n",rv,inet_ntop(AF_INET,
                        &servaddr.sin_addr, buf, 4096));
        }
        Connect (connfd, (sockaddr *) &servaddr, sizeof(servaddr));
        */

        connfd = Tcp_Connect (argv[1], port);

        str_cli (stdin, connfd);

    }
    catch (const std::exception& e) {
        std::cout << "Exception: " << e.what() << "\n";
    }
}


