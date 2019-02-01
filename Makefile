PROJ_NAME = libwordcompression.so
EXECUTABLE = wordcompressor
SRC = $(filter-out ./src/main.c,$(wildcard ./src/*.c))
H_DIR =./include
HEADER = ./include/libwordcompression.h
CC = gcc
LD_LIBRARY_PATH = $LD_LIBRARY_PATH
CFLAGS = -Wall -I$(H_DIR) -std=c99 -lutf8proc -D_GNU_SOURCE
all: $(PROJ_NAME) main

main:
	$(CC) $(SRC) ./src/main.c -o $(EXECUTABLE) $(CFLAGS)

$(PROJ_NAME):
	$(CC) $(SRC) -shared -fPIC -o $@ $(CFLAGS)

install: $(PROJ_NAME)
	cp $(PROJ_NAME) /usr/local/lib
	chmod 777 /usr/local/lib/$(PROJ_NAME)
	cp $(EXECUTABLE) /usr/local/bin
	chmod 777 /usr/local/bin/$(EXECUTABLE)
	cp $(HEADER) /usr/local/include/

clean:
	rm -rf ./src/*.o $(PROJ_NAME)
