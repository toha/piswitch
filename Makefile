
CROSS_COMPILE=arm-linux-gnueabi-
ARCH=arm 
CC=${CROSS_COMPILE}gcc-4.6

all: piswitch

piswitch: http.o gpio.o
		$(CC) src/piswitch.c -o bin/piswitch

http.o: src/http/http.c
		$(CC) src/http/http.c

gpio.o: modela.o modelb.o
		$(CC) src/gpio/gpio.c 

modela.o: src/gpio/protocols/modela.c
		$(CC) src/gpio/protocols/modela.c

modelb.o: src/gpio/protocols/modelb.c
		$(CC) src/gpio/protocols/modelb.c


clean:
		rm -rf *o bin/piswitch
