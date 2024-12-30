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

    // Duplicate the file descriptor
    int fd_copy = dup(fd);
    if (fd_copy == -1) {
        perror("dup");
        return 1;
    }

    // Write using both descriptors
    dprintf(fd, "Hello from original descriptor\n");
    dprintf(fd_copy, "Hello from duplicate descriptor\n");

    // Close descriptors
    close(fd);
    close(fd_copy);

    return 0;
}
