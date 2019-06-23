KERN_DIR=/home/dr/raspberry_src/linux-rpi-4.14.y
all:
	make -C $(KERN_DIR) M='pwd' modules ARCH=arm CROSS_COMPILE=/home/dr/raspberry_src/tools-master/arm-bcm2708/arm-linux-gnueabihf/bin/arm-linux-gnueabihf-
clearn:
	rm -f *.ko *.o *.mod.o *mod.c *.symvers modul* modules.order
obj-m += led_drv.o
