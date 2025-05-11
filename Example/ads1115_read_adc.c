#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>

#define ADS1115_IOCTL_MAGIC 'a'
#define ADS1115_IOCTL_READ_ADC _IOR(ADS1115_IOCTL_MAGIC, 4, int)

int main(void) {
    int ads_fd;
    int adc_val;

    // Mở thiết bị ADS1115
    ads_fd = open("/dev/ads1115", O_RDWR);
    if (ads_fd < 0) {
        perror("Failed to open ADS1115 device");
        return -1;
    }

    while (1) {
        // Đọc giá trị ADC
        if (ioctl(ads_fd, ADS1115_IOCTL_READ_ADC, &adc_val) < 0) {
            perror("Failed to read ADC");
            close(ads_fd);
            return -1;
        }
        printf("ADC Value: %d\n", adc_val);
        sleep(1);  // Chờ 1 giây trước khi đọc lại
    }

    close(ads_fd);
    return 0;
}
