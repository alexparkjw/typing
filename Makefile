TARGETS = type

CROSS_TOOL = 
CC_CPP = $(CROSS_TOOL)g++
CC_C = $(CROSS_TOOL)gcc-8
CC_CLANG = $(CROSS_TOOL)gcc

CFLAGS = -Wall -g -std=c99 -Werror -lpthread -lncurses

all: clean $(TARGETS)

$(TARGETS):
	$(CC_C) $(CFLAGS) $@.c -o $@

clean:
	rm -f $(TARGETS)

