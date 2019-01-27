SRC=$(wildcard source/*.c)

all:
	gcc -o word-compression $(SRC) -lutf8proc -Wall -g
clean:
	rm -rf word-compression