SRC=$(wildcard source/*.c)

all:
	gcc -o word-compression $(SRC) -lutf8proc -Wall -O3
clean:
	rm -rf word-compression
