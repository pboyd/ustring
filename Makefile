AR = ar
CC = gcc
CFLAGS = -Iinclude -O2 -Wall

.PHONY: all clean test examples

all: lib/libustring.a

src/utf8.o: src/utf8.c include/ustring.h
	$(CC) $(CFLAGS) -c src/utf8.c -o $@

src/utf16.o: src/utf16.c include/ustring.h
	$(CC) $(CFLAGS) -c src/utf16.c -o $@

lib/libustring.a : src/utf8.o src/utf16.o
	$(AR) rcs $@ src/utf8.o src/utf16.o

clean:
	rm lib/libustring.a src/*.o ;\
		rm -rf doc ; \
		rm examples/utf16_print examples/utf8_print

test: lib/libustring.a
	$(CC) $(CFLAGS) src/test/*.c -o ustring.test -Llib -lustring && \
		./ustring.test ; rm ustring.test

doc: include/ustring.h
	doxygen

examples: examples/utf8_print examples/utf16_print

examples/utf8_print: examples/utf8_print.c lib/libustring.a
	$(CC) $(CFLAGS) examples/utf8_print.c -o examples/utf8_print -Llib -lustring

examples/utf16_print: examples/utf16_print.c lib/libustring.a
	$(CC) $(CFLAGS) examples/utf16_print.c -o examples/utf16_print -Llib -lustring
