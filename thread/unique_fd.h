#ifndef UNIQUE_FD_H
#define UNIQUE_FD_H

#include <errno.h>
#include <unistd.h>
#include <utility>
using std::move;

class unique_fd {
    int fd;
public:
    int get() { return fd; }
    unique_fd( int _fd) : fd{_fd} {}
    ~unique_fd() noexcept { 
        if (-1 != fd) {
            int retval;
loop:
            retval = close (fd);
            if (retval < 0) {
                if (EINTR == errno) // interrupted, try again
                    goto loop;
            }
        }
    }

    //moves, which suppress copy defaults
    unique_fd(unique_fd&& old) : fd{old.fd} {old.fd = -1;}
    unique_fd& operator= (unique_fd&& old) {
        fd = old.fd;
        old.fd = -1;
    }

};

#endif
