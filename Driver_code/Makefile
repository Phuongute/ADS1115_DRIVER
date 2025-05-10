obj-m += ads1115_ioctl.o gpio_driver.o
KDIR = /lib/modules/$(shell uname -r)/build

all:
	make -C $(KDIR) M=$(shell pwd) modules
clean:
	make -C $(KDIR) M=$(shell pwd) clean
install:
	sudo insmod ads1115_ioctl.ko
	sudo insmod gpio_driver.ko
uninstall:
	sudo rmmod ads1115_ioctl
	sudo rmmod gpio_driver.ko
	make clean
check:
	dmesg -w
sys:
	ls /sys/bus/i2c/devices
