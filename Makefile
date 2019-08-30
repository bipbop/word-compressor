PROJ_NAME = libwordcompression.so
PROJ_VERSION = 1
EXECUTABLE = wordcompressor
SRC = $(filter-out ./src/main.c,$(wildcard ./src/*.c))
H_DIR =./include
HEADER = ./include/libwordcompression.h
CC = gcc
LD_LIBRARY_PATH = $LD_LIBRARY_PATH
CFLAGS = -g -Wall -I$(H_DIR) -std=c99 -lutf8proc -D_GNU_SOURCE -ansi -pedantic
all: $(PROJ_NAME) main

main:
	$(CC) $(SRC) ./src/main.c -o $(EXECUTABLE) $(CFLAGS)

$(PROJ_NAME):
	$(CC) $(SRC) -shared -fPIC -o $@ $(CFLAGS)

install: $(PROJ_NAME) main
	cp $(PROJ_NAME) /usr/local/lib/$(PROJ_NAME).$(PROJ_VERSION)
	ln -sf /usr/local/lib/$(PROJ_NAME).$(PROJ_VERSION) /usr/local/lib/$(PROJ_NAME)
	cp $(EXECUTABLE) /usr/local/bin
	gcc -DISHEADER=1 -I./include/ -E include/libwordcompression.h > /usr/local/include/libwordcompression.h

clean:
	rm -rf ./src/*.o $(PROJ_NAME) $(EXECUTABLE)
