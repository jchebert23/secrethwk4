CC     = gcc
CFLAGS = -g3 -std=c99 -pedantic -Wall
HWK    = /c/cs323/Hwk4

encodeAndDecode: encodeAndDecode.o
	${CC} ${CFLAGS} -o $@ $^ 

encodeAndDecode.o: encodeAndDecode.c hash.c stack.c

