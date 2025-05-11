# DRIVER ADS1115
Driver này cung cấp giao tiếp với ADS1115, một bộ chuyển đổi ADC 16-bit 4 kênh từ Texas Instruments, thông qua I2C interface trên nền tảng Linux.
Nó cung cấp một giao diện thiết bị ký tự (/dev/ads1115) để người dùng tương tác thông qua các lệnh ioctl() cho cấu hình, chọn kênh ADC, đọc giá trị, và thiết lập ngưỡng cảnh báo.
## 1.Yêu cầu hệ thống

## 2.Thông số kỹ thuật (Specifications)

- **Độ phân giải:** 16-bit (±32768 mức)
- **Số kênh:** 4 kênh đầu vào analog (single-ended) hoặc 2 kênh vi sai (differential)
- **Tốc độ lấy mẫu (Data Rate):** 8 SPS đến 860 SPS
- **Giao tiếp:** I2C (100kHz hoặc 400kHz)
- **Ngưỡng cảnh báo:** Thiết lập thông qua thanh ghi `LO_THRESH` và `HI_THRESH`
- **Điện áp hoạt động:** 2.0V – 5.5V
- **Chân ngắt (ALERT):** Kích hoạt khi giá trị ADC vượt ngưỡng


## 3.ADS1115 Pinout

| Pin | Tên        | Loại       | Mô tả                                     |
|-----|------------|------------|-------------------------------------------|
| 1   | VDD        | Nguồn cấp  | Điện áp cấp nguồn (2.0V – 5.5V)           |
| 2   | GND        | GND        | Mass (0V)                                 |
| 3   | SCL        | I2C Clock  | Dây cấp xung Clock I2C                    |
| 4   | SDA        | I2C Data   | Tín hiệu dữ liệu I2C                      |
| 5   | ALERT      | Output     | Chân ngắt khi vượt ngưỡng                 |
| 6   | ADDR       | Input      | Cấu hình địa chỉ I2C (ADDR)               |
| 7   | AIN0       | Analog In  | Kênh đầu vào analog 0                     |
| 8   | AIN1       | Analog In  | Kênh đầu vào analog 1                     |
| 9   | AIN2       | Analog In  | Kênh đầu vào analog 2                     |
| 10  | AIN3       | Analog In  | Kênh đầu vào analog 3                     |


## 4.Tải & Cài đặt:
### Kiểm tra wiringPi
**wiringPi** thường được cài sẵn trong Raspbian OS.  
Kiểm tra bằng lệnh sau:
```
gpio -v
Nếu chưa có, cài đặt:
```
sudo apt update
sudo apt install wiringpi
Tải và biên dịch driver:
```
git clone https://github.com/Phuongute/HTN.git
cd HTN/Driver_code
make
sudo make install
Sử dụng trong chương trình của bạn
Thêm header:
```
#include <ads1115_ioctl.h>
#include <ads1115_Driver.h>
Biên dịch chương trình:
```
gcc test_ads1115_1.c ads1115_Driver.c ads1115_ioctl.c gpio_driver.c -lwiringPi -o run

## 5.Hướng dẫn sử dụng:

## 6.Mô tả các hàm:

## 7.Sử dụng Example
# Test Driver ADS1115 Trên Raspberry Pi 3B

Repo này chứa 2 chương trình chạy ở user-space để test chức năng đọc ADC và cảnh báo ALERT từ IC ADS1115, dùng Raspberry Pi 3B.

## File Chức Năng

| Tên File                 | Mô Tả                                                                 |
|--------------------------|----------------------------------------------------------------------|
| `ads1115_read_adc.c`     | Đọc giá trị ADC liên tục từ kênh AIN0 (có thể chỉnh sang kênh khác). |
| `ads1115_alert_monitor.c`| Thiết lập ngưỡng ALERT, đọc trạng thái chân ALERT qua GPIO17.        |

## Cách sử dụng Terminal
File 1: Cách chọn channel để đọc

## Cách biên Dịch trên Terminal
gcc ads1115_read_adc.c -o ads1115_read_adc
gcc ads1115_alert_monitor.c -o ads1115_alert_monitor
## Cách chạy trên Terminal
sudo ./ads1115_read_adc
sudo ./ads1115_alert_monitor

### 7.1.Tải và biên dịch Example






