#include "max7219.h"
#include <wiringPiSPI.h>
#include <math.h>
#include <wiringPi.h>
#define SPI_CHANNEL 0
#define SPI_SPEED   1000000

void max7219_send(uint8_t address, uint8_t data) {
    uint8_t buffer[2] = {address, data};
    wiringPiSPIDataRW(SPI_CHANNEL, buffer, 2);
}

void init_max7219(uint8_t decode_mode, uint8_t intensity, uint8_t scan_limit) {
    wiringPiSPISetup(SPI_CHANNEL, SPI_SPEED);
    
    max7219_send(MAX7219_REG_SHUTDOWN, 0x01);
    max7219_send(MAX7219_REG_DISPLAYTEST, 0x00);
    max7219_send(MAX7219_REG_SCANLIMIT, scan_limit & 0x07);
    max7219_send(MAX7219_REG_DECODEMODE, decode_mode);
    max7219_set_intensity(intensity);
    max7219_clear();
    for (int row = 0; row < 8; row++) {
        max7219_send(row + 1, 0xFF);
        delay(200);
        max7219_send(row + 1, 0x00);
    }
}

void max7219_clear(void) {
    for (uint8_t i = MAX7219_REG_DIGIT0; i <= MAX7219_REG_DIGIT7; i++) {
        max7219_send(i, 0x00);
    }
}

void max7219_set_intensity(uint8_t intensity) {
    max7219_send(MAX7219_REG_INTENSITY, intensity & 0x0F);
}

void max7219_display_number(int num) {
    for (uint8_t i = MAX7219_REG_DIGIT0; i <= MAX7219_REG_DIGIT7; i++) {
        max7219_send(i, num % 10);
        num /= 10;
        if (num == 0) break;
    }
}
void display(float data){
    uint8_t segmentData[10] = {0x7E, 0x30, 0x6D, 0x79, 0x33, 0x5B, 0x5F, 0x70, 0x7F, 0x7B};
    uint8_t segment [8] = {};
    int int_data = (int)roundf(data*100);
    segment[4] = segmentData[int_data/1000];
    segment[5] = segmentData[(int_data/100) % 10] | 0x80;
    segment[6] = segmentData[(int_data/10) % 10];
    segment[7] = segmentData[int_data % 10];
    for(int i = 0; i < 8; i++)
        max7219_send(i + 1, segment[7 - i]);  
}