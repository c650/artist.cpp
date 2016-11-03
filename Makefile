CC=g++-5
CFLAGS= --std=c++11 -Wall -Werror
SRC = main.cpp
OUT = artist.o

build: ${SRC}
	${CC} ${CFLAGS} -o ${OUT} ${SRC}

.PHONY: clean

clean:
	rm *.o
