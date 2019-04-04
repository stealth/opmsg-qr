CC=gcc
AR=ar
RANLIB=ranlib
CFLAGS=-Wall -O2 -std=c11 -pedantic

.PHONY: all clean

all: libqrencode.a

libqrencode.a: bitmask.o mask.o mmask.o mqrspec.o qrencode.o qrinput.o qrspec.o rsecc.o split.o
	$(AR) cr libqrencode.a *.o
	$(RANLIB) libqrencode.a

bitmask.o: bitstream.c
	$(CC) $(CFLAGS) -c bitstream.c

mask.o: mask.c
	$(CC) $(CFLAGS) -c mask.c

mmask.o: mmask.c
	$(CC) $(CFLAGS) -c mmask.c

mqrspec.o: mqrspec.c
	$(CC) $(CFLAGS) -c mqrspec.c

qrencode.o: qrencode.c
	$(CC) $(CFLAGS) -c qrencode.c

qrinput.o: qrinput.c
	$(CC) $(CFLAGS) -c qrinput.c

qrspec.o: qrspec.c
	$(CC) $(CFLAGS) -c qrspec.c

rsecc.o: rsecc.c
	$(CC) $(CFLAGS) -c rsecc.c

split.o: split.c
	$(CC) $(CFLAGS) -c split.c

clean:
	rm -rf *.o

