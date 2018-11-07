CC     = gcc
CFLAGS = -g3 -std=c99 -pedantic -Wall
HWK    = /c/cs323/Hwk4




encode: lzw.o /c/cs323/Hwk4/code.o
	${CC} ${CFLAGS} -o $@ $^



#decode: lzw.o /c/cs323/Hwk4/code.o
#	ln -f encode decode


lzw.o: lzw.c hash.c stack.c /c/cs323/Hwk4/code.h
	${CC} ${CFLAGS} -c $<

