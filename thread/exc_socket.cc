#include "exc_socket.h"
#include "unique_fd.h"

#include <system_error>
#include <string.h>

#include <netdb.h>

socket_t Socket(int domain, int type, int protocol) {
    int retval = socket(domain, type, protocol);
    if (retval > 0)
        return socket_t{retval};
    else 
        throw std::system_error(errno, std::system_category(), "socket error:");
}

void Bind(socket_t socket, const struct sockaddr *address, socklen_t
        address_len) {
    int retval = bind(socket, address, address_len);
    if (retval < 0)
        throw std::system_error(errno, std::system_category(), "bind error:");
}

void Listen(socket_t socket, int backlog) {
    int retval = listen(socket, backlog);
    if (retval < 0)
        throw std::system_error(errno, std::system_category(),
                "listen error: ");
}

//socket_t Accept(socket_t socket, struct sockaddr *restrict address, socklen_t
//        *restrict address_len) {
// g++/clang specific restrict keyword
socket_t Accept(socket_t socket, struct sockaddr *__restrict__  address, socklen_t
        *__restrict__  address_len) {
    int retval = accept(socket, address, address_len);
    if (retval > 0)
        return socket_t{retval};
    else 
        throw std::system_error(errno, std::system_category(),
                "accept error: ");
}

void Connect(socket_t socket, const struct sockaddr *address, socklen_t
        address_len) {
    int retval = connect(socket, address, address_len);
    if (retval < 0)
        throw std::system_error(errno, std::system_category(),
                "connect error: ");
}

void Close (socket_t socket) {
    int retval;
loop: 
    retval = close (socket);
    if (retval < 0) {
        if (EINTR == errno) // interrupted, try again
            goto loop;
        else if (EIO == errno) // filesystem problem
           throw std::system_error(errno, std::system_category(),
                   "close error: ");
    }
    // don't throw just for EBADF
     //   throw std::system_error(errno, std::system_category());
}

pid_t Fork() {
    int retval = fork();
    if (retval < 0)
        throw std::system_error(errno, std::system_category(),
                "fork error: ");
    else
        return retval;
}

pid_t Waitpid(pid_t pid, int *stat_loc, int options) {
    int retval = waitpid (pid, stat_loc, options);
    if (retval < 0)
        throw std::system_error(errno, std::system_category(), "waitpid error: ");
    else
        return retval;
}

void Inet_pton(int af, const string& src, void *dst) {
    int retval = inet_pton (af, src.c_str(), dst);
    if (retval == 0) 
        throw std::runtime_error("Inet_pton: invalid network address");
    if (retval == -1)
        throw std::logic_error("Inet_pton: invalid address family");
}

socket_t Tcp_Bind ( int port, int listeners) {

    socket_t listenfd = Socket ( AF_INET, SOCK_STREAM, 0);

    sockaddr_in servaddr;
    socklen_t address_len = sizeof(servaddr);

    memset (&servaddr, 0, address_len);
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl (INADDR_ANY);
    servaddr.sin_port = htons (port);

    try {
        Bind (listenfd, (sockaddr *) &servaddr, address_len);
        Listen (listenfd, listeners);
    }
    catch (...) {

        Close (listenfd);
        throw;
    }

    return listenfd;

}

socket_t Tcp_Connect ( const string & address_s, int port) {
    socket_t connfd = Socket (AF_INET, SOCK_STREAM, 0);

    /*
    sockaddr_in servaddr; 
    socklen_t address_len = sizeof (servaddr);

    memset (&servaddr, 0, address_len);
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons (port);
    Inet_pton (AF_INET, address_s, &servaddr.sin_addr);
    */

    // more cribbing from UNP, figure 11.10 tcp_connect
    struct addrinfo hints, *res, *ressave;

    memset (&hints, 0, sizeof(addrinfo));
    hints.ai_family = AF_INET; // less general
    // AF_UNSPEC gives Exception: connect error: : Address family not
    // supported by protocol
    hints.ai_socktype = SOCK_STREAM;

    int n;
    if ((n = getaddrinfo (address_s.c_str(), nullptr, &hints, &res)) != 0)
        throw std::system_error(n, std::system_category(),
                gai_strerror(n) );
    ressave = res;

    // end UNP crib; leaving off some of its power for simplicity

    try {
        reinterpret_cast<sockaddr_in*> (res->ai_addr)->sin_port = htons (port);
        Connect (connfd, res->ai_addr, res->ai_addrlen);
        //Connect (connfd, (sockaddr *) &servaddr, address_len);
    }
    catch (...) {
        Close (connfd);
        freeaddrinfo (ressave);
        throw;
    }

    freeaddrinfo (ressave);
    return connfd;
}
