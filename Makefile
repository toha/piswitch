MKDIR_P = mkdir -p

OUT_DIR=build
SRC=src

CROSS_COMPILE=arm-linux-gnueabi-
ARCH=arm 
CC=${CROSS_COMPILE}gcc-4.6

CFLAGS=-c -Wall

all: builddir piswitch

piswitch: http.o gpio.o
		$(CC) $(SRC)/piswitch.c $(OUT_DIR)/http.o $(OUT_DIR)/gpio.o -o $(OUT_DIR)/piswitch

http.o: 
		$(CC) $(CFLAGS) $(SRC)/http/http.c -o $(OUT_DIR)/http.o

gpio.o: modela.o modelb.o
		$(CC) $(CFLAGS) $(SRC)/gpio/gpio.c -o $(OUT_DIR)/gpio.o

modela.o: 
		$(CC) $(CFLAGS) $(SRC)/gpio/protocols/modela.c -o $(OUT_DIR)/modela.o

modelb.o:
		$(CC) $(CFLAGS) $(SRC)/gpio/protocols/modelb.c -o $(OUT_DIR)/modelb.o

# create build dir
builddir: ${OUT_DIR}

${OUT_DIR}:
		$(MKDIR_P) ${OUT_DIR}

clean:
		rm -rf *o build/

.PHONY: builddir
