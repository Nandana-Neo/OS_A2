#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>    // for open
#include <unistd.h>   // for close, read, write

#define DEVICE_PATH "/dev/os_driver_device"

int main() {
    int fd;
    char write_buf[256];
    char read_buf[256] = {0};

    // Open the device file for read/write
    fd = open(DEVICE_PATH, O_RDWR);
    if (fd < 0) {
        perror("Failed to open the device");
        return EXIT_FAILURE;
    }

    // Prompt the user for input
    printf("Write to the device: ");
    if (fgets(write_buf, sizeof(write_buf), stdin) == NULL) {
        perror("Failed to read input");
        close(fd);
        return EXIT_FAILURE;
    }
    // Remove the newline character from input if it exists
    write_buf[strcspn(write_buf, "\n")] = '\0';

    // Write to the device
    if (write(fd, write_buf, strlen(write_buf)) < 0) {
        perror("Failed to write to the device");
        close(fd);
        return EXIT_FAILURE;
    }
    printf("Wrote to the device: %s\n", write_buf);

    // Read from the device
    if (read(fd, read_buf, sizeof(read_buf)) < 0) {
        perror("Failed to read from the device");
        close(fd);
        return EXIT_FAILURE;
    }
    printf("Read from the device: %s\n", read_buf);

    // Close the device file
    close(fd);
    return EXIT_SUCCESS;
}

