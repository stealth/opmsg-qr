CC=gcc
AR=ar
RANLIB=ranlib
CFLAGS=-Wall -O2 -std=c11 -pedantic

.PHONY: all clean build

all: build libqrencode.a

build:
	mkdir build || true

libqrencode.a: build/qrenc.o build/bitmask.o build/mask.o build/mmask.o build/mqrspec.o build/qrencode.o build/qrinput.o build/qrspec.o build/rsecc.o build/split.o
	$(AR) cr build/libqrencode.a build/*.o
	$(RANLIB) build/libqrencode.a

build/bitmask.o: bitstream.c
	$(CC) $(CFLAGS) -c bitstream.c -o build/bitmask.o

build/mask.o: mask.c
	$(CC) $(CFLAGS) -c mask.c -o build/mask.o

build/mmask.o: mmask.c
	$(CC) $(CFLAGS) -c mmask.c -o build/mmask.o

build/mqrspec.o: mqrspec.c
	$(CC) $(CFLAGS) -c mqrspec.c -o build/mqrspec.o

build/qrencode.o: qrencode.c
	$(CC) $(CFLAGS) -c qrencode.c -o build/qrencode.o

build/qrenc.o: qrenc.c
	$(CC) $(CFLAGS) -c qrenc.c -o build/qrenc.o

build/qrinput.o: qrinput.c
	$(CC) $(CFLAGS) -c qrinput.c -o build/qrinput.o

build/qrspec.o: qrspec.c
	$(CC) $(CFLAGS) -c qrspec.c -o build/qrspec.o

build/rsecc.o: rsecc.c
	$(CC) $(CFLAGS) -c rsecc.c -o build/rsecc.o

build/split.o: split.c
	$(CC) $(CFLAGS) -c split.c -o build/split.o

clean:
	rm -rf build

