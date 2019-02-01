PROJ_NAME = wordcompression.so
C_SOURCE = $(wildcard ./src/*.c)
H_SOURCE = $(wildcard ./include/*.h)
H_DIR =./include
OBJ = $(C_SOURCE:.c=.o)
CC = gcc
CFLAGS = -c -W -Wall -ansi -pedantic -I$(H_DIR) -I/usr/lib  -I/usr/local/lib -lutf8proc

all: $(PROJ_NAME)
 
$(PROJ_NAME): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^
 
%.o: %.c %.h
	$(CC) $(CFLAGS) -shared -o $@ $<
 
clean:
	rm -rf *.o $(PROJ_NAME) *~
