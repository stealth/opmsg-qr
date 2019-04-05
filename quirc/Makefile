# quirc -- QR-code recognition library
# Copyright (C) 2010-2012 Daniel Beer <dlbeer@gmail.com>
#
# Permission to use, copy, modify, and/or distribute this software for any
# purpose with or without fee is hereby granted, provided that the above
# copyright notice and this permission notice appear in all copies.
#
# THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
# WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
# MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
# ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
# WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
# ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
# OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.

CC=cc
CFLAGS=-O2 -Wall -std=c11 -pedantic

QUIRC_CFLAGS = -Ilib $(CFLAGS)
LIB_OBJ = \
    lib/decode.o \
    lib/identify.o \
    lib/quirc.o \
    lib/version_db.o
DEMO_OBJ = \
    demo/camera.o \
    demo/mjpeg.o \
    demo/convert.o \
    demo/dthash.o \
    demo/scanner.o

all: libquirc.a

libquirc.a: $(LIB_OBJ) $(DEMO_OBJ)
	rm -f $@
	ar cr $@ $(LIB_OBJ) $(DEMO_OBJ)
	ranlib $@

.c.o:
	$(CC) $(QUIRC_CFLAGS) -o $@ -c $<

clean:
	rm -f */*.o
	rm -f libquirc.a

