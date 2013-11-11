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

SRCS=$(SRC)/gpio/protocols/modela.c \
	 $(SRC)/gpio/protocols/modelb.c

OBJS=$(SRCS:.c=.o)

all: builddir piswitch

piswitch: $(OBJS) 
	$(CC) $(CFLAGS) $(PIFLAGS) $(SRC)/piswitch.c $(SRC)/lib/rcswitch-pi/RCSwitch.cpp -o $(OUT_DIR)/$(NAME) -ldl -L/usr/local/lib -lwiringPi -lwiringPiDev -lpthread -lm

# create build dir
builddir: ${OUT_DIR}

${OUT_DIR}:
	$(MKDIR_P) ${OUT_DIR}

clean:
	@- $(RM) $(OBJS)
	@- $(RM) src/lib/mongoose/mongoose.a
	@- $(RM) src/http/http.o
	@- $(RM) -rf $(OUT_DIR)

.PHONY: builddir all clean
