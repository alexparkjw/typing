TARGETS = type

CROSS_TOOL = 
CC_CPP = $(CROSS_TOOL)g++
CC_C = $(CROSS_TOOL)gcc-8
CC_CLANG = $(CROSS_TOOL)gcc

CFLAGS = -Wall -g -std=c99 -Werror -lpthread

# curses compile and run
CURSES = 
GTK = -I/usr/local/include -L/usr/local/lib -lncurses

# GTK compile and run
GTK = -I/usr/local/include -L/usr/local/lib `pkg-config --cflags --libs gtk+-2.0` 

# SDL compile and run
SDL = -I/usr/local/include -L/usr/local/lib -lSDL2 -lSDL2_image -lgtk+-2 

# OpenGL compile and run
OpenGL = -I/usr/local/include -L/usr/local/Cellar/glfw/3.3/lib -lglfw -lglew  -framework Cocoa -framework OpenGL -framework IOKit -framework CoreVideo

all: clean $(TARGETS)

$(TARGETS):
	$(CC_CLANG) $@.c -o $@  $(CFLAGS)

clean:
	rm -f $(TARGETS)

