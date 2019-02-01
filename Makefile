PROJ_NAME = libwordcompression.so

SRC = $(wildcard ./src/*.c)
H_DIR =./include
CC = gcc
CFLAGS = -Wall -I$(H_DIR) -std=c99 -lutf8proc -D_GNU_SOURCE -fPIC

all: $(PROJ_NAME)

$(PROJ_NAME): $(OBJ)
	$(CC) $(CFLAGS) $(SRC) -shared -o $@ $^

clean:
	rm -rf ./src/*.o $(PROJ_NAME)
