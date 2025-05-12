# DRIVER ADS1115
Driver này cung cấp giao tiếp với ADS1115, một bộ chuyển đổi ADC 16-bit 4 kênh từ Texas Instruments, thông qua I2C interface trên nền tảng Linux.
Nó cung cấp một giao diện thiết bị ký tự (/dev/ads1115) để người dùng tương tác thông qua các lệnh ioctl() cho cấu hình, chọn kênh ADC, đọc giá trị, và thiết lập ngưỡng cảnh báo.

## Mục lục

1. [Yêu cầu hệ thống](#1yêu-cầu-hệ-thống)  
   - [Phần cứng](#phần-cứng)  
   - [Phần mềm](#phần-mềm)  

2. [Thông số kỹ thuật (Specifications)](#2thông-số-kỹ-thuật-specifications)

3. [ADS1115 Pinout](#3ads1115-pinout)

4. [Tải & Cài đặt](#4tải--cài-đặt)  

5. [Hướng dẫn sử dụng](#5hướng-dẫn-sử-dụng)

6. [Mô tả các hàm](#6mô-tả-các-hàm)

7. [Example - Test Driver ADS1115 Trên Raspberry Pi 3B](#7example---test-driver-ads1115-trên-raspberry-pi-3b)  
   - [Hướng dẫn cách chọn channel](#71hướng-dẫn-cách-chọn-channel)  
   - [Cách biên dịch và chạy Example](#72cách-biên-dịch-và-chạy-example)


## 1.Yêu cầu hệ thống
Để sử dụng driver ADS1115 này, hệ thống cần đảm bảo các yêu cầu sau:

Phần cứng: 
Raspberry Pi (ưu tiên Raspberry Pi 3B).
Module ADS1115 (I2C ADC 16-bit 4 kênh từ Texas Instruments).
Kết nối I2C hoạt động đúng (SCL/SDA đến đúng chân trên Raspberry Pi).

Phần mềm:
Cần đảm bảo Gói kernel headers phù hợp với phiên bản kernel này: 5.10.92-v7+
Trước khi sử dụng ADS1115, giao tiếp I2C phải được bật:
```
sudo raspi-config
# Vào mục "Interfacing Options" → chọn "I2C" → chọn "Enable"
sudo reboot
```
Kiểm tra thiết bị I2C đã kết nối
Đảm bảo module ADS1115 được nhận diện trên bus I2C (thường là /dev/i2c-1):
```
sudo apt install -y i2c-tools
i2cdetect -y 1
```
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
Để sử dụng được Driver trên, bạn phải có sẵn raspberry-kernel-header có phiên bản theo mục yêu cầu hệ thống

4.1. Tải driver:
```
git clone https://github.com/Phuongute/HTN.git
```
4.2. Thêm driver vào device tree:
Sau khi cài đặt driver, bạn cần đưa file .dtbo (trong thư mục Bin) vào thư mục overlay để kernel có thể tải đúng thiết bị.
```
sudo dtc -@ -I dts -O dtb -o ads1115-overlay.dtbo ads1115-overlay.dts # Biên dịch thành file dtbo trước rồi thêm vào overlay
sudo cp ads1115-overlay.dtbo /boot/overlays/
```
4.3. Sau đó vào /boot/config.txt để thêm cho kernel biết sẽ build driver cùng kúc boot
```
sudo nano /boot/config.txt
dtoverlay=ads1115-overlay #Thêm dòng này vào cuối file
```
Sau đó khởi động lại Raspberry
```
sudo reboot
```
4.4. Cách biên dịch và cài đặt driver
```
cd HTN/Driver_code
make
sudo make install
```

## 5.Hướng dẫn sử dụng:

## 6.Mô tả các hàm:

## 7.Example
# Test Driver ADS1115 Trên Raspberry Pi 3B

Repo này chứa 2 chương trình chạy ở user-space để test chức năng đọc ADC và cảnh báo ALERT từ IC ADS1115, dùng Raspberry Pi 3B.

## File Chức Năng

| Tên File                 | Mô Tả                                                                 |
|--------------------------|----------------------------------------------------------------------|
| `ads1115_read_adc.c`     | Đọc giá trị ADC liên tục từ kênh AIN0 (có thể chỉnh sang kênh khác). |
| `ads1115_alert_monitor.c`| Thiết lập ngưỡng ALERT, đọc trạng thái chân ALERT qua GPIO17.        |

### 7.1.Hướng dẫn cách chọn channel
Ví dụ test file ads1115_read_adc.c
```
// Single-ended input modes (AINx vs GND)
#define ADS1115_MUX_0_GND  4  // AIN0 - GND
#define ADS1115_MUX_1_GND  5  // AIN1 - GND
#define ADS1115_MUX_2_GND  6  // AIN2 - GND
#define ADS1115_MUX_3_GND  7  // AIN3 - GND
int16_t channel = ADS1115_MUX_0_GND;
```
Thay biến channel bằng 1 trong 4 define trên để chọn kênh tương ứng.

Ví dụ test file ads1115_alert_monitor.c

int16_t hiThresh = (int16_t)(3*32768.0/4.096f);   // Thay 3 bằng giá trị ngưỡng trên

int16_t loThresh = (int16_t)(1*32768.0/4.096f);   // Thay 1 bằng giá trị ngưỡng dưới

### 7.2.Cách biên dịch và chạy Example
Trong thư mục Example, có các chương trình mẫu mà bạn có thể biên dịch và chạy. Sử dụng câu lệnh
```
gcc ads1115_alert_monitor.c -o ads1115_alert_monitor 
gcc ads1115_read_adc.c -o ads1115_read_adc           
sudo ./ads1115_read_adc
sudo ./ads1115_alert_monitor
```

### 7.3. Sơ đồ nối chân để test
Đối với ads1115_read_adc.c
![image](https://github.com/user-attachments/assets/1fb527fa-4744-49d6-8509-baa98257f250)

Đối với ads1115_alert_monitor.c
Nối chân 17 (BCM tương ứng chân 11 rào cắm) với chân ALRT 



