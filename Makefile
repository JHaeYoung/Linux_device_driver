obj-m += bh1750.o
 
all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules
 
clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean

CFLAGS += -I /usr/src/linux-headers-6.1.21-v8+
