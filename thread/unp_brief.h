#include <sys/types.h>
#include <unistd.h>
#include <errno.h>

const int MAXLINE=4096;

ssize_t  Readline(int, void *, size_t);
ssize_t  Readn(int, void *, size_t);
void     Writen(int, void *, size_t);
