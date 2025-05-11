# i2c1602

A Raspberry Pi C library to write LCD 16x2 with I2C using WiringPi

This library is based mainly on [Lewis Loflin's code](https://www.example.com).  
It uses wiringPi I2C to interface with an I2C 16x2 LCD, which has a  
I2C converter module attached in the back.

## To clone and install this library:

Make sure to have wiringPi installed.  
**RaspbianOS has wiringPi pre-installed by default.**  
Check by typing `gpio -v` in terminal. If there is a version then wiringPi is installed.
