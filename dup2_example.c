#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

int main() {
    // Open file for writing
    int fd = open("output.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd == -1) {
        perror("open");
        return 1;
    }

    // Duplicate file descriptor onto stdout
    if (dup2(fd, STDOUT_FILENO) == -1) {
        perror("dup2");
        return 1;
    }

    // Write to stdout (redirected to the file)
    printf("This will be written to output.txt\n");

    // Close the file descriptor
    close(fd);

    return 0;
}
