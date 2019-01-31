SRC=$(wildcard src/*.c)

all:
	gcc -o word-compression.so $(SRC) -lutf8proc -Wall -I include -O2 -shared
clean:
	rm -rf word-compression