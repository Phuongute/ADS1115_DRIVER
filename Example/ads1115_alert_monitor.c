#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>

#define GPIO_MAGIC 'G'
#define GPIO_GET_PIN _IOR(GPIO_MAGIC, 1, int)
#define ADS1115_IOCTL_MAGIC 'a'
#define ADS1115_IOCTL_SET_HITHRESH _IOW(ADS1115_IOCTL_MAGIC, 3, int)
#define ADS1115_IOCTL_SET_LOTHRESH _IOW(ADS1115_IOCTL_MAGIC, 2, int)

int main(void) {
    int fd, ads_fd;
    int value;
    int hiThresh = 3;  // Ngưỡng ALERT cao
    int loThresh = 1;  // Ngưỡng ALERT thấp
    int alert_pin = 17;  // GPIO pin cho ALERT

    // Mở thiết bị GPIO và ADS1115
    fd = open("/dev/gpio_ioctl_device", O_RDWR);
    if (fd < 0) {
        perror("Failed to open GPIO device");
        return -1;
    }

    ads_fd = open("/dev/ads1115", O_RDWR);
    if (ads_fd < 0) {
        perror("Failed to open ADS1115 device");
        return -1;
    }

    // Cấu hình ngưỡng ALERT của ADS1115
    if (ioctl(ads_fd, ADS1115_IOCTL_SET_HITHRESH, &hiThresh) < 0) {
        perror("Failed to set high threshold");
        close(ads_fd);
        close(fd);
        return -1;
    }
    if (ioctl(ads_fd, ADS1115_IOCTL_SET_LOTHRESH, &loThresh) < 0) {
        perror("Failed to set low threshold");
        close(ads_fd);
        close(fd);
        return -1;
    }

    while (1) {
        // Đọc trạng thái chân ALERT
        if (ioctl(fd, GPIO_GET_PIN, &value) < 0) {
            perror("Failed to get GPIO pin value");
            close(ads_fd);
            close(fd);
            return -1;
        }

        printf("GPIO %d ALERT state: %d\n", alert_pin, value);
        sleep(1);  // Chờ 1 giây trước khi kiểm tra lại
    }

    close(ads_fd);
    close(fd);
    return 0;
}
