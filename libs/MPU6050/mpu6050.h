#ifndef MPU6050_H
    #define MPU6050_H
    #include <stdio.h>
    #include <wiringPi.h>
    #include <wiringPiSPI.h>
    #include <stdint.h>
    #include <unistd.h>
    #define _USE_MATH_DEFINES
    #include <math.h>   //gcc ... -lwiringPi -lm

    #define ADDR 0x68
    #define MPU_REG25_SAMPLE_RATE 25
    #define MPU_REG26_CONFIGURATION 26
    #define MPU_REG27_GYROSCOPE_CONF 27
    #define MPU_REG27_ACCELEROMETER_CONF 28
    #define MPU_REG56_INTERRUPT_ENABLE 56
    #define MPU_REG107_PWR_MANAGEMENT_1 107
    #define ACC_X 59
    #define ACC_Y 61
    #define ACC_Z 63    
    #define TEMP 65

    int mpu;
    float Temp_sensor(void);
    int16_t read_sensor(uint8_t sensor);
    void Init_MPU6050(void);
    float Acc_sensor(uint8_t MODE,int Range);
#endif