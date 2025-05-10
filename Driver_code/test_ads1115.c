#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <errno.h> // Include errno header

#define DEVICE_PATH "/dev/ads1115"
// IOCTL commands
#define ADS1115_IOCTL_MAGIC 'a'

#define ADS1115_IOCTL_CONFIG _IOW(ADS1115_IOCTL_MAGIC, 1, int)
#define ADS1115_IOCTL_SET_LOTHRESH _IOW(ADS1115_IOCTL_MAGIC, 2, int)
#define ADS1115_IOCTL_SET_HITHRESH _IOW(ADS1115_IOCTL_MAGIC, 3, int)
#define ADS1115_IOCTL_READ_ADC _IOR(ADS1115_IOCTL_MAGIC, 4, int)

int main() {
    int fd;
    int data = 0xC383;
    int adc_val;
    

    // Open the device
    fd = open(DEVICE_PATH, O_RDWR);
    if (fd < 0) {
        perror("Failed to open the device");
        return errno;
    }

    // Config ADS1115
    if (ioctl(fd, ADS1115_IOCTL_CONFIG, &data) < 0) {
        perror("Failed to config ADS1115");
        close(fd);
        return errno;
    }
    ioctl(fd, ADS1115_IOCTL_READ_ADC, &adc_val);
    printf("ADC value = %d\n", adc_val);
    close(fd);
    return 0;
}
//sudo dtc -@ -I dts -O dtb -o ads1115-overlay.dtbo ads1115-overlay.dts
