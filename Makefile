MKDIR_P = mkdir -p
NAME=piswitch
OUT_DIR=build
SRC=src
#CROSS_COMPILE=arm-linux-gnueabi-
ARCH=arm
#CC=${CROSS_COMPILE}gcc
CC=gcc

PIFLAGS=-Isrc/lib/mongoose -pthread -g
#CFLAGS=-W -Wall -fpermissive
CFLAGS=-W -Wall -std=c99
LDFLAGS=-ldl -L/usr/local/lib -lwiringPi -lwiringPiDev -lpthread -lm -ljansson

SRCS=$(SRC)/rf/rx.c $(SRC)/rf/tx.c $(SRC)/rf/protocol.c $(SRC)/rf/protocol1.c $(SRC)/rf/protocol2.c $(SRC)/rf/protocol3.c $(SRC)/rf/protocol4.c $(SRC)/rf/protocol5.c $(SRC)/http/http.c $(SRC)/lib/mongoose/mongoose.c $(SRC)/helper.c
OBJS=$(SRC)/rf/rx.o $(SRC)/rf/tx.o $(SRC)/rf/protocol.o $(SRC)/rf/protocol1.o $(SRC)/rf/protocol2.o $(SRC)/rf/protocol3.o $(SRC)/rf/protocol4.o $(SRC)/rf/protocol5.o $(SRC)/http/http.o $(SRC)/lib/mongoose/mongoose.o $(SRC)/helper.o

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
