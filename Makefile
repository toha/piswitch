MKDIR_P = mkdir -p

NAME=piswitch
OUT_DIR=build
SRC=src
CROSS_COMPILE=arm-linux-gnueabi-
ARCH=arm 
CC=${CROSS_COMPILE}gcc

CFLAGS=-Isrc/lib/mongoose -W -Wall -pthread -g

SRCS=$(SRC)/gpio/protocols/modela.c \
		 $(SRC)/gpio/protocols/modelb.c \
		 $(SRC)/gpio/gpio.c \
		 $(SRC)/http/http.c

OBJS=$(SRCS:.c=.o)

all: builddir piswitch

piswitch: $(OBJS)
		$(CC) $(CFLAGS) $(SRC)/piswitch.c src/lib/mongoose/mongoose.c $(OBJS) -o $(OUT_DIR)/$(NAME) -ldl

# create build dir
builddir: ${OUT_DIR}

${OUT_DIR}:
		$(MKDIR_P) ${OUT_DIR}

clean:
		@- $(RM) $(OBJS)
		@- $(RM) src/lib/mongoose/mongoose.a
		@- $(RM) $(OUT_DIR)/$(NAME)

.PHONY: builddir all clean
