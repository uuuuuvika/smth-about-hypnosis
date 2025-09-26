CC=cc
PRO=pro

SRC=main.c src/load_gif_frames.c src/load_text.c src/display_gif_frames.c src/display_text_ff.c
OBJ=$(SRC:.c=.o)

RGB_LIB_DISTRIBUTION=../rpi-rgb-led-matrix
RGB_INCDIR=$(RGB_LIB_DISTRIBUTION)/include
RGB_LIBDIR=$(RGB_LIB_DISTRIBUTION)/lib
RGB_LIBRARY_NAME=rgbmatrix
RGB_LIBRARY=$(RGB_LIBDIR)/lib$(RGB_LIBRARY_NAME).a

MAGICK_CFLAGS=$(shell pkg-config --cflags MagickWand)
MAGICK_LDFLAGS=$(shell pkg-config --libs MagickWand)

CFLAGS=-Wall -O3 -g -Wextra -Wno-unused-parameter
CFLAGS+=-I$(RGB_INCDIR) $(MAGICK_CFLAGS)

LDFLAGS+=$(RGB_LIBRARY) -lrt -lm -lpthread -lstdc++ $(MAGICK_LDFLAGS)

all: $(PRO)

$(PRO): $(OBJ) $(RGB_LIBRARY)
	$(CC) $(OBJ) -o $@ $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

$(RGB_LIBDIR)/lib$(RGB_LIBRARY_NAME).a: FORCE
	$(MAKE) -C $(RGB_LIB_DISTRIBUTION)

clean:
	rm -f $(OBJ) $(PRO)

fclean: clean
	$(MAKE) -C $(RGB_LIB_DISTRIBUTION) clean
	rm -f $(PRO)

FORCE:

.PHONY: all clean FORCE fclean