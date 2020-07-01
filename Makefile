AR = ar
CC = gcc
CFLAGS = -Iinclude -O2 -Wall

.PHONY: all clean test

all: lib/libustring.a

src/utf8.o: src/utf8.c include/ustring.h
	$(CC) $(CFLAGS) -c src/utf8.c -o $@

src/utf16.o: src/utf16.c include/ustring.h
	$(CC) $(CFLAGS) -c src/utf16.c -o $@

lib/libustring.a : src/utf8.o src/utf16.o
	$(AR) rcs $@ src/utf8.o src/utf16.o

clean:
	rm lib/libustring.a src/*.o ; rm -rf doc

test: lib/libustring.a
	$(CC) $(CFLAGS) src/test/*.c -o ustring.test -Llib -lustring && \
		./ustring.test ; rm ustring.test

doc: include/ustring.h
	doxygen
