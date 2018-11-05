CC     = gcc
CFLAGS = -g3 -std=c99 -pedantic -Wall
HWK    = /c/cs323/Hwk4

decode: decode.o
	${CC} ${CFLAGS} -o $@ $^ 

encode.o: encode.c hash.c

decode.o: decode.c hash.c stack.c
