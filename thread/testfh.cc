#include "unique_fd.h"
#include <unistd.h>
#include <cstdio>
#include <utility>

#include <fcntl.h>

void pw(unique_fd fd) {
    write(fd.get(),"lalala\n",7);
}

int main() {
    int f = open("test",O_WRONLY|O_CREAT);
    perror(nullptr);
    unique_fd fd(f);
    printf("fd %d %d\n",f, fd.get());
    write(fd.get(), "aooo\n",5);
    write(1, "booo\n",5);
    {
        unique_fd fout(1);
        write(1, "cooo\n",5);
        pw(std::move(fout));
    }
    pw(std::move(fd));
    write(1, "dooo\n",5);
    perror(nullptr);
    //close(1);
    write(1, "eooo\n",5);
}
