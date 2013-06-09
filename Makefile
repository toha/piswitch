MKDIR_P = mkdir -p

NAME=piswitch
OUT_DIR=build
SRC=src
CROSS_COMPILE=arm-linux-gnueabi-
ARCH=arm 
CC=${CROSS_COMPILE}gcc

PIFLAGS=-Isrc/lib/mongoose -pthread -g
CFLAGS=-W -Wall

SRCS=$(SRC)/gpio/protocols/modela.c \
	 $(SRC)/gpio/protocols/modelb.c \
	 $(SRC)/gpio/gpio.c 

OBJS=$(SRCS:.c=.o)

all: builddir piswitch

piswitch: $(OBJS) 
	$(CC) $(CFLAGS) $(PIFLAGS) $(SRC)/piswitch.c src/http/http.c src/lib/mongoose/mongoose.c -o $(OUT_DIR)/$(NAME) -ldl
#	$(CC) $(CFLAGS) $(SRC)/piswitch.c $(OBJS) $(SRC)/http/http.o -o $(OUT_DIR)/$(NAME)


# create build dir
builddir: ${OUT_DIR}

${OUT_DIR}:
	$(MKDIR_P) ${OUT_DIR}

clean:
	@- $(RM) $(OBJS)
	@- $(RM) src/lib/mongoose/mongoose.a
	@- $(RM) -rf $(OUT_DIR)

.PHONY: builddir all clean
