#include <stdio.h>
#include <string.h>
#include <fcntl.h>

void cat(int fd, int outfd) {
    int bufferSize = 1024;
    char buffer[bufferSize];
    memset(buffer, 0, bufferSize);
    int readBytes = 0;
    while((readBytes = read(fd, buffer, bufferSize)) > 0) {
        int sendBytes = 0;
        while (sendBytes < readBytes) {
            int writeBytes = write(1, buffer + sendBytes, readBytes - sendBytes);
            if (writeBytes < 0) {
                fprintf(stderr, "Writing error\n");
            }
            sendBytes += writeBytes;
        }
    }
    if (readBytes < 0) {
        fprintf(stderr, "Reading error\n");
    }
}

int main(int argc, char** argv) {
    if (argc > 1) {
        int i;
        for (i = 1; i < argc; i++) {
            int fd = open(argv[i], O_RDONLY);
            if (fd < 0) {
                fprintf(stderr, "Opening error");
                continue;
            } else {
                cat(fd, 1);
                close(fd);
            }
        }
    } else {
        cat(0, 1);
    }
    return 0;
}
