#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>

int main(int argc, char * varg[]) {
    int inputFd, outputFd, openFlags;
    size_t bufferSize = 1024;
    char buffer[bufferSize];
    mode_t mode;

errno = 0;
    if (argc != 3) {
        errno = EINVAL;
        perror("Bad arguments");
        exit(EXIT_FAILURE);
    }

    openFlags = O_RDONLY;
    inputFd = open(varg[1], openFlags);
    if (inputFd == -1) {
        perror("Input file error");
        exit(EXIT_FAILURE);
    }

    openFlags = O_WRONLY | O_CREAT | O_TRUNC;
    outputFd = open(varg[2], openFlags, S_IRUSR | S_IWUSR);
    if (outputFd == -1) {
        perror("Output file error");
        close(inputFd);
        exit(EXIT_FAILURE);
    }

    ssize_t bytesRead;
    while ((bytesRead = read(inputFd, buffer, bufferSize)) > 0) {
        if (write(outputFd, buffer, bytesRead) != bytesRead) {
            perror("Write error");
            close(inputFd);
            close(outputFd);
            exit(EXIT_FAILURE);
        }
    }

    if (bytesRead == -1) {
        perror("Read error");
    }

    close(inputFd);
    close(outputFd);

    if (remove(varg[1]) == -1) {
        perror("Remove error");
        exit(EXIT_FAILURE);
    }
    exit(0);
}
