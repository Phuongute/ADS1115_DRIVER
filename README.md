# DRIVER ADS1115
Driver này cung cấp giao tiếp với ADS1115, một bộ chuyển đổi ADC 16-bit 4 kênh từ Texas Instruments, thông qua I2C interface trên nền tảng Linux.
Nó cung cấp một giao diện thiết bị ký tự (/dev/ads1115) để người dùng tương tác thông qua các lệnh ioctl() cho cấu hình, chọn kênh ADC, đọc giá trị, và thiết lập ngưỡng cảnh báo.

## Mục lục

1. [Yêu cầu hệ thống](#1yêu-cầu-hệ-thống)  

2. [Thông số kỹ thuật (Specifications)](#2thông-số-kỹ-thuật-specifications)

3. [ADS1115 Pinout](#3ads1115-pinout)

4. [Tải & Cài đặt](#4tải--cài-đặt)  

5. [Mô tả các hàm](#5mô-tả-các-hàm)

6. [Examples](#6Examples)

7. [Các file tài liệu tham khảo](#7-Các-file-tài-liệu-tham-khảo)

[Thành viên phát triển Driver-ADS1115](#Thành-viên-phát-triển-Driver-ADS1115)  

## 1.Yêu cầu hệ thống
Để sử dụng driver ADS1115 này, hệ thống cần đảm bảo các yêu cầu sau:

**Phần cứng:**
- Raspberry Pi (ưu tiên Raspberry Pi 3B).

- Module ADS1115 (I2C ADC 16-bit 4 kênh từ Texas Instruments).

- Kết nối I2C hoạt động đúng (SCL/SDA đến đúng chân trên Raspberry Pi).

**Phần mềm:**
- Cần đảm bảo Gói kernel headers phù hợp với phiên bản kernel này: 5.10.92-v7+

  **!! Nếu sử dụng phiên bản khác, cần phải sửa một số thay đổi trong driver mới có thể hoạt động**
- Trước khi sử dụng ADS1115, giao tiếp I2C phải được bật:
```
sudo raspi-config
Vào mục "Interfacing Options" → chọn "I2C" → chọn "Enable"
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

**Để sử dụng được Driver trên, bạn phải có sẵn raspberry-kernel-header có phiên bản theo mục yêu cầu hệ thống**

- Tải driver:
```
git clone https://github.com/Phuongute/HTN.git
```
- Thêm driver vào device tree:
Sau khi cài đặt driver, bạn cần đưa file .dtbo (trong thư mục Bin) vào thư mục overlay để kernel có thể tải đúng thiết bị.
```
sudo dtc -@ -I dts -O dtb -o ads1115-overlay.dtbo ads1115-overlay.dts # Biên dịch thành file dtbo trước rồi thêm vào overlay
sudo cp ads1115-overlay.dtbo /boot/overlays/
```
- Sau đó vào /boot/config.txt để thêm cho kernel biết sẽ build driver cùng lúc boot
```
sudo nano /boot/config.txt
dtoverlay=ads1115-overlay #Thêm dòng này vào cuối file
```
- Sau đó khởi động lại Raspberry
```
sudo reboot
```
**Cách biên dịch và cài đặt driver**
```
cd HTN/Driver_code
make
sudo make install
```

## Hướng dẫn thay đổi địa chỉ trên ads1115


ADS1115 hỗ trợ tối đa **4 địa chỉ I2C khác nhau**, cho phép kết nối nhiều IC ADS1115 trên cùng một bus I2C. Địa chỉ này được xác định thông qua cách kết nối chân **ADDR** của chip:

| Kết nối chân ADDR   | Địa chỉ I2C (hex) |
|---------------------|-------------------|
| GND                 | `0x48`            |
| VDD                 | `0x49`            |
| SDA                 | `0x4A`            |
| SCL                 | `0x4B`            |

1. **Chọn địa chỉ mong muốn** (ví dụ: `0x49`).
2. **Kết nối chân ADDR** đến chân tương ứng:
3. **Kiểm tra bằng lệnh`i2cdetect`**:
   i2cdetect -y 1

## 5.Mô tả các hàm:
| Hàm                                                             | Mô tả                                                                                                                                                                                                                                                                                                                                                |
| --------------------------------------------------------------- | ---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- |
| `ads1115_set_config(struct i2c_client *client, u16 config)`     | Ghi cấu hình (register 0x01) vào ADS1115. Dữ liệu được truyền từ user space thông qua ioctl `ADS1115_IOCTL_CONFIG`.                                                                                                                                                                                                                                  |
| `ads1115_set_loThresh(struct i2c_client *client, u16 loThresh)` | Ghi giá trị ngưỡng thấp (`LO_THRESH` – thanh ghi 0x02) vào ADS1115 để cấu hình ngưỡng cảnh báo dưới. Sau đó đọc lại để xác nhận.                                                                                                                                                                                                                     |
| `ads1115_set_hiThresh(struct i2c_client *client, u16 hiThresh)` | Ghi giá trị ngưỡng cao (`HI_THRESH` – thanh ghi 0x03) để thiết lập ngưỡng cảnh báo trên.                                                                                                                                                                                                                                                                |
| `ads1115_read_adc(struct i2c_client *client)`                   | Đọc dữ liệu ADC từ thanh ghi chuyển đổi (`CONVERSION REGISTER` – 0x00). Trả về giá trị ADC 16-bit (dạng signed).                                                                                                                                                                                                                                     |
| `ads1115_set_channel(struct i2c_client *client, u16 channel)`   | Đọc lại thanh ghi cấu hình, thay đổi giá trị MUX để chọn kênh đầu vào (AINx), sau đó kích hoạt chuyển đổi. Dùng ioctl `ADS1115_IOCTL_SET_CHANNEL`.                                                                                                                                                                                                   |
| `ads1115_ioctl(...)`                                            | Xử lý các lệnh ioctl từ chương trình không gian người dùng: <br>– `ADS1115_IOCTL_CONFIG`: Cấu hình ADC.<br>– `ADS1115_IOCTL_SET_LOTHRESH`: Thiết lập ngưỡng thấp.<br>– `ADS1115_IOCTL_SET_HITHRESH`: Thiết lập ngưỡng cao.<br>– `ADS1115_IOCTL_SET_CHANNEL`: Chọn kênh ADC.<br>– `ADS1115_IOCTL_READ_ADC`: Đọc giá trị ADC và trả về cho user space. |

## 6.Examples
Test Driver ADS1115 Trên Raspberry Pi 3B
Repo này chứa 2 chương trình chạy ở user-space để test chức năng đọc ADC và cảnh báo ALERT từ IC ADS1115, dùng Raspberry Pi 3B.
2 chương trình này được lưu ở: https://github.com/Phuongute/HTN/tree/main/Example
## File Chức Năng

| Tên File                 | Mô Tả                                                                 |
|--------------------------|----------------------------------------------------------------------|
| `ads1115_read_adc.c`     | Đọc giá trị ADC liên tục từ kênh AIN0 (có thể chỉnh sang kênh khác). |
| `ads1115_alert_monitor.c`| Thiết lập ngưỡng ALERT, đọc trạng thái chân ALERT qua GPIO17.        |

### 6.1.Hướng dẫn cách chọn channel
**Ví dụ test file ads1115_read_adc.c**
```
// Single-ended input modes (AINx vs GND)
#define ADS1115_MUX_0_GND  4  // AIN0 - GND
#define ADS1115_MUX_1_GND  5  // AIN1 - GND
#define ADS1115_MUX_2_GND  6  // AIN2 - GND
#define ADS1115_MUX_3_GND  7  // AIN3 - GND
int16_t channel = ADS1115_MUX_0_GND;
```
- Thay biến channel bằng 1 trong 4 define trên để chọn kênh tương ứng.

**Ví dụ test file ads1115_alert_monitor.c**
```
int16_t hiThresh = (int16_t)(3*32768.0/4.096f);   // Thay 3 bằng giá trị ngưỡng trên

int16_t loThresh = (int16_t)(1*32768.0/4.096f);   // Thay 1 bằng giá trị ngưỡng dưới
```

### 6.2.Cách biên dịch và chạy Example
Trong thư mục Example, có các chương trình mẫu mà bạn có thể biên dịch và chạy. Sử dụng câu lệnh
```
gcc ads1115_alert_monitor.c -o ads1115_alert_monitor 
gcc ads1115_read_adc.c -o ads1115_read_adc           
sudo ./ads1115_read_adc
sudo ./ads1115_alert_monitor
```

### 6.3. Sơ đồ nối chân để test
**Đối với ads1115_read_adc.c**
![image](https://github.com/user-attachments/assets/1fb527fa-4744-49d6-8509-baa98257f250)

**Đối với ads1115_alert_monitor.c**
- Nối chân 17 (BCM tương ứng chân 11 rào cắm) với chân ALRT 
## 7. Các file tài liệu tham khảo

- Bạn có thể truy cập vào thư mục Docs, đây là nơi chứa các tài liệu tham khảo về các hàm GPIO, I2C ở trong linux kernel. Đồng thời cũng có kèm theo cả file datasheet của ADS1115

Truy cập tại đây:

'''
https://github.com/Phuongute/HTN/tree/main/Docs
'''


## Thành viên phát triển Driver ADS1115
### MAI THANH PHƯƠNG 22146376
### ĐINH ĐỒNG SƠN 22146390
### NGUYỄN PHẠM TRUNG QUÂN 22146384





