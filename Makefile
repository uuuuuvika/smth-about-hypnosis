CC=cc
PROJECT=project

SRC=main.c
OBJ=$(SRC:.c=.o)

RGB_LIB_DISTRIBUTION=../rpi-rgb-led-matrix
RGB_INCDIR=$(RGB_LIB_DISTRIBUTION)/include
RGB_LIBDIR=$(RGB_LIB_DISTRIBUTION)/lib
RGB_LIBRARY_NAME=rgbmatrix
RGB_LIBRARY=$(RGB_LIBDIR)/lib$(RGB_LIBRARY_NAME).a

CFLAGS=-Wall -O3 -g -Wextra -Wno-unused-parameter
CFLAGS+=-I$(RGB_INCDIR)

LDFLAGS+=$(RGB_LIBRARY) -lrt -lm -lpthread -lstdc++

all: $(PROJECT)

$(PROJECT): $(OBJ) $(RGB_LIBRARY)
	$(CC) $(OBJ) -o $@ $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

$(RGB_LIBRARY): FORCE
	$(MAKE) -C $(RGB_LIB_DISTRIBUTION)

clean:
	rm -f $(OBJ) $(PROJECT)

FORCE:

.PHONY: all clean FORCE