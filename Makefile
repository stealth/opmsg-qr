

OPMSG=`pwd`/../src

CXX=c++
CXXFLAGS=-Wall -O2 -pedantic -std=c++11
INC=-Ilibqrencode -I$(OPMSG)
LIB=libqrencode/build/libqrencode.a quirc/libquirc.a -lcrypto -lm -ljpeg

OBJS=$(OPMSG)/build/keystore.o $(OPMSG)/build/config.o $(OPMSG)/build/misc.o $(OPMSG)/build/brainkey.o\
     $(OPMSG)/build/deleters.o $(OPMSG)/build/marker.o $(OPMSG)/build/base64.o $(OPMSG)/build/missing.o


.PHONY: all clean libqrencode quirc

all: libqrencode quirc opmsg-qr

clean:
	rm -rf *.o
	make -C libqrencode clean
	make -C quirc clean


libqrencode:
	make -C libqrencode

quirc:
	make -C quirc


opmsg-qr: opmsg-qr.o
	$(CXX) opmsg-qr.o $(OBJS) $(LIB) -o opmsg-qr

opmsg-qr.o: opmsg-qr.cc
	$(CXX) $(INC) $(CXXFLAGS) -c opmsg-qr.cc

