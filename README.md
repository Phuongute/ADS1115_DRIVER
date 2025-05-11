# ADS1115 Kernel Module Driver (I2C + IOCTL)
## Mô tả
Đây là driver kernel dạng character device dành cho module **ADS1115** (I2C ADC 16-bit của Texas Instruments).  
Driver này hỗ trợ giao tiếp với thiết bị thông qua **IOCTL**, cho phép cấu hình các thanh ghi, đọc giá trị ADC, và chọn kênh đọc thông qua file `/dev/ads1115`.
---
## Yêu cầu hệ thống
- Linux Kernel 5.x – 6.x (đã test với 6.10 / 6.12)
- Module `i2c-dev` được bật
- Board có I2C (ví dụ Raspberry Pi, BeagleBone, hoặc custom embedded)
---
## Cấu trúc file
- `ioctl.c`: source code kernel module
- `Makefile`: để biên dịch module
- `test_ads1115.c`: chương trình userspace ví dụ để giao tiếp
---
## Build
```bash
make
#Load / Unload driver
#Tải driver vào kernel: sudo insmod ads1115.ko
#Kiểm tra:
dmesg | grep ads1115
ls /dev/ads1115
#Gỡ driver:
sudo rmmod ads1115




