CC=g++
CFLAGS= --std=c++11 -Wall -Werror
SRC = main.cpp bitmap.cpp
OUT = artist.o

build: ${SRC}
	${CC} ${CFLAGS} -o ${OUT} ${SRC}

.PHONY: clean

clean:
	rm *.o
