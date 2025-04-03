#include "mpu6050.h"
#include <wiringPiI2C.h>
#include <math.h>   //gcc ... -lwiringPi -lm

int16_t read_sensor(uint8_t sensor){
    int16_t high, low, data;
    high = wiringPiI2CReadReg8(mpu,sensor);
    low = wiringPiI2CReadReg8(mpu,sensor + 1);
    data = (high<<8) | low;
    return data;
}

float Temp_sensor(void){
    return (float)read_sensor(TEMP)/340.0 + 36.53;
}
float Acc_sensor(uint8_t MODE,int Range){
    float Sen = (Range == 2)? 16384.0 : (Range == 4)? 8192.0 : (Range == 8)? 4096.0 :
                (Range == 16)? 2048.0 : 0;
    return (float)read_sensor(MODE)/Sen;
}


void Init_MPU6050(void){
    //setup I2C
    mpu = wiringPiI2CSetup(ADDR);
    // register 25->28,56,107
    // sample rate 1kHz
    wiringPiI2CWriteReg8(mpu, MPU_REG25_SAMPLE_RATE, 0);
    // không sử dụng nguồn xung ngoài, bat DLFP 94Hz
    wiringPiI2CWriteReg8(mpu, MPU_REG26_CONFIGURATION, 2);    
    // gyro FS: +-1000 độ/s
    wiringPiI2CWriteReg8(mpu, MPU_REG27_GYROSCOPE_CONF, 0x16);
    // acc FS: +- 2g
    wiringPiI2CWriteReg8(mpu, MPU_REG27_ACCELEROMETER_CONF, 0);
    // // mở interrupt của dataready
    // wiringPiI2CWriteReg8(mpu, Interrupt, 1);
    // // chọn nguồn xung Gyro X
    // wiringPiI2CWriteReg8(mpu, PWR_managment, 0x01);
    wiringPiI2CWriteReg8(mpu, MPU_REG107_PWR_MANAGEMENT_1, 0);
    printf("SUCCESSFUL!\n");
}