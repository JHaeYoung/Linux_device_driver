#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>

int main() {
	int fd = open("/dev/bh1750", O_RDONLY);
    if (fd < 0) {
        perror("Failed to open device file");
        return -1;
    }

    int data;
    ssize_t bytes_read = read(fd, &data, sizeof(data));
    if (bytes_read != sizeof(data)) {
        perror("Failed to read data from device");
        close(fd);
        return -1;
    }

    printf("Data read from bh1750 sensor: %d\n", data);

    close(fd);
    return 0;
}
