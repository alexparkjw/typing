TARGETS = main

CC = clang

BREW_PREFIX    = $(shell brew --prefix)
NCURSES_PREFIX = $(shell brew --prefix ncurses)

CFLAGS   = -Wall -g -std=c99 -Werror
INCLUDES = -I$(NCURSES_PREFIX)/include
LDFLAGS  = -L$(NCURSES_PREFIX)/lib -lncurses -lpthread

# GTK compile and run
GTK = -I$(BREW_PREFIX)/include -L$(BREW_PREFIX)/lib `pkg-config --cflags --libs gtk+-2.0`

# SDL compile and run
SDL = -I$(BREW_PREFIX)/include -L$(BREW_PREFIX)/lib -lSDL2 -lSDL2_image

# OpenGL compile and run
OpenGL = -I$(BREW_PREFIX)/include \
         -L$(shell brew --prefix glfw)/lib -lglfw \
         -L$(shell brew --prefix glew)/lib -lGLEW \
         -framework Cocoa -framework OpenGL -framework IOKit -framework CoreVideo

RM = rm -f

.PHONY: all run clean

all: $(TARGETS)

run: $(TARGETS)
	./$(TARGETS)

$(TARGETS): %: %.c include/Header.h include/sem.h
	$(CC) $@.c -o $@ $(INCLUDES) $(CFLAGS) $(LDFLAGS)

clean:
	$(RM) $(TARGETS)
