AR = ar
CC = gcc
CFLAGS = -Iinclude -O2 -Wall

.PHONY: all clean test

all: lib/libustring.a

src/utf8.o: src/utf8.c include/ustring.h
	$(CC) $(CFLAGS) -c src/utf8.c -o $@

lib/libustring.a : src/utf8.o
	$(AR) rcs $@ src/utf8.o

clean:
	rm lib/libustring.a src/*.o

test: lib/libustring.a
	$(CC) $(CFLAGS) src/test/*.c -o ustring.test -Llib -lustring && \
		./ustring.test ; rm ustring.test
