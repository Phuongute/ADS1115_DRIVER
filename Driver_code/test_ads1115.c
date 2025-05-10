#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <errno.h> // Include errno header

#define DEVICE_PATH "/dev/ads1115"
#define ads1115_IOCTL_MAGIC 'a'
#define ads1115_IOCTL_CONFIG _IOW(ads1115_IOCTL_MAGIC, 3, int)  

int main() {
    int fd;
    int data = 0xC383;

    // Open the device
    fd = open(DEVICE_PATH, O_RDWR);
    if (fd < 0) {
        perror("Failed to open the device");
        return errno;
    }

    // Config ADS1115
    if (ioctl(fd, ads1115_IOCTL_CONFIG, &data) < 0) {
        perror("Failed to config ADS1115");
        close(fd);
        return errno;
    }
    
    close(fd);
    return 0;
}
//sudo dtc -@ -I dts -O dtb -o ads1115-overlay.dtbo ads1115-overlay.dts
