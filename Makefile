TARGETS = main

CROSS_TOOL = 
CC_CPP = $(CROSS_TOOL)g++-10
CC_GCC = $(CROSS_TOOL)gcc-10
CC_CLANG = $(CROSS_TOOL)clang

CFLAGS = -Wall -g -std=c99 -Werror -lpthread

# Ncurses compile and run
NCURSES = -I/usr/local/include/6.1include -L/usr/local/Cellar/ncurses/6.1/lib -lncurses 

# GTK compile and run
GTK = -I/usr/local/include -L/usr/local/lib `pkg-config --cflags --libs gtk+-2.0` 

# SDL compile and run
SDL = -I/usr/local/include -L/usr/local/lib -lSDL2 -lSDL2_image -lgtk+-2 

# OpenGL compile and run
OpenGL = -I/usr/local/include -L/usr/local/Cellar/glfw/3.3/lib -lglfw -lglew  -framework Cocoa -framework OpenGL -framework IOKit -framework CoreVideo

RM = rm -f

all: $(TARGETS)

$(TARGETS):
	$(CC_CLANG) $@.c -o $@  $(CFLAGS) $(NCURSES)

clean:
	$(RM) $(TARGETS)

