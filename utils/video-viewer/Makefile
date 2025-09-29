CXXFLAGS = -O3 -W -Wall -Wextra -Wno-unused-parameter -D_FILE_OFFSET_BITS=64

# Point this to your rpi-rgb-led-matrix repo
RGB_LIB_DISTRIBUTION ?= /home/pi/py/rpi-rgb-led-matrix
RGB_INCDIR = $(RGB_LIB_DISTRIBUTION)/include
RGB_LIBDIR = $(RGB_LIB_DISTRIBUTION)/lib
RGB_LIBRARY = $(RGB_LIBDIR)/librgbmatrix.a
RGB_LDFLAGS = -L$(RGB_LIBDIR) -lrgbmatrix -lrt -lm -lpthread

AV_CXXFLAGS := $(shell pkg-config --cflags libavcodec libavformat libswscale libavutil libavdevice)
AV_LDFLAGS := $(shell pkg-config --cflags --libs libavcodec libavformat libswscale libavutil libavdevice)

all: video-viewer

$(RGB_LIBRARY):
	$(MAKE) -C $(RGB_LIBDIR)

video-viewer.o: video-viewer.cc
	$(CXX) -I$(RGB_INCDIR) $(CXXFLAGS) $(AV_CXXFLAGS) -c $< -o $@

video-viewer: video-viewer.o $(RGB_LIBRARY)
	$(CXX) $(CXXFLAGS) video-viewer.o -o $@ $(RGB_LDFLAGS) $(AV_LDFLAGS)

clean:
	rm -f video-viewer.o video-viewer