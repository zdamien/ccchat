// TCP echo server.  Largely typed in from UNP 3e,
// apart from using my own function wrappers.

#include "exc_socket.h"
#include "unique_fd.h"

#include <string>
#include <iostream>
#include <cstring>
#include <unistd.h>
#include <sys/wait.h>

const int SERV_PORT = 4000;
const int MAXLINE = 4096;

void str_echo (unique_fd sockfd) {
    ssize_t n;
    char buf[MAXLINE];

    while ( (n = recv (sockfd.get(), buf, MAXLINE, 0)) > 0) {
        // assuming this just works, because it's a dumb echo server nor
        // worthy of a sendall()
        send(sockfd.get(), buf, n, 0); 
    }
}

int main(int argc, char** argv) {
    int port = SERV_PORT;

    if (argc == 2)
        port = atoi (argv[1]);

    try {
        unique_fd listenfd ( Tcp_Bind (port) );

        while (true) {
            struct sockaddr_in cliaddr;
            socklen_t clilen = sizeof (cliaddr);
            unique_fd connfd ( Accept (listenfd.get(), (sockaddr *)
                        &cliaddr, &clilen));

            // double fork daemon idiom
            pid_t child_pid=-1;
            if ( (child_pid = Fork()) == 0) {
                if ( (child_pid = Fork()) == 0) {
                    listenfd.~unique_fd();  // close listener handle
                    str_echo (move(connfd));
                    exit(0);
                } else 
                    exit(0);
            }
            // wait for the short-lived intermediate child
            Waitpid(child_pid, nullptr, 0);
        }
    }
    catch (const std::exception& e) {
        std::cout << "Exception: " << e.what() << "\n";
    }
}


