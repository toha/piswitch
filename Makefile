MKDIR_P = mkdir -p
NAME=piswitch
OUT_DIR=build
SRC=src
#CROSS_COMPILE=arm-linux-gnueabi-
ARCH=arm 
#CC=${CROSS_COMPILE}gcc
CC=g++

PIFLAGS=-Isrc/lib/mongoose -pthread -g
CFLAGS=-W -Wall -fpermissive
LDFLAGS=-ldl -L/usr/local/lib -lwiringPi -lwiringPiDev -lpthread -lm

SRCS=$(SRC)/rf/receive.c $(SRC)/lib/rcswitch-pi/RCSwitch.cpp $(SRC)/http/http.c $(SRC)/lib/mongoose/mongoose.c $(SRC)/helper.c
OBJS=$(SRC)/rf/receive.o $(SRC)/lib/rcswitch-pi/RCSwitch.o $(SRC)/http/http.o $(SRC)/lib/mongoose/mongoose.o $(SRC)/helper.o

all: builddir piswitch

piswitch: $(OBJS) 
	$(CC) $(CFLAGS) $(LDFLAGS) $(PIFLAGS) $(SRC)/piswitch.c -o $(OUT_DIR)/$(NAME) $(OBJS) 

# create build dir
builddir: ${OUT_DIR}

${OUT_DIR}:
	$(MKDIR_P) ${OUT_DIR}

clean:
	@- $(RM) $(OBJS)
	@- $(RM) -rf $(OUT_DIR)

.PHONY: builddir all clean
