// Exception-throwing wrappers to socket and other system functions,
// capitalized a la Unix Network Programming.

#ifndef CCCHAT_EXC_SOCKET_H
#define CCCHAT_EXC_SOCKET_H

#include <sys/socket.h>
#include <sys/wait.h>

#include <netinet/in.h>  /* sockaddr_in{} and other Internet defns */
#include <arpa/inet.h>   /* inet(3) functions */

#include <unistd.h>
#include <string>
using std::string;


typedef int socket_t;  //might wrap file descriptors in a handle later

socket_t Socket(int domain, int type, int protocol);

void Bind(socket_t socket, const struct sockaddr *address, socklen_t address_len);

void Listen(socket_t socket, int backlog);

//socket_t Accept(socket_t socket, struct sockaddr *restrict address, socklen_t *restrict address_len);
// ^ man page declaration, using C 'restrict' which isn't C++ ^
socket_t Accept(socket_t socket, struct sockaddr *address, socklen_t *address_len);

void Connect(socket_t socket, const struct sockaddr *address, socklen_t address_len);

void Close (socket_t socket);

pid_t Fork ();

pid_t Waitpid(pid_t pid, int *stat_loc, int options);

void Inet_pton(int af, const char *src, void *dst);

socket_t Tcp_Bind ( int port, int listeners=20);

socket_t Tcp_Connect ( const string & address_s, int port) ;


#endif
