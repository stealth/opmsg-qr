/**
 * qrencode - QR Code encoder
 *
 * QR Code encoding tool
 * Copyright (C) 2006-2017 Kentaro Fukuchi <kentaro@fukuchi.org>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>
#include <errno.h>

#include "qrencode.h"

#define INCHES_PER_METER (100.0/2.54)

static int casesensitive = 1;
static int eightbit = 0;
static int version = 0;
static int size = 3;
static int margin = -1;
static int dpi = 72;
static int structured = 0;
static int rle = 0;
static int svg_path = 0;
static int micro = 0;
static int inline_svg = 0;
static QRecLevel level = QR_ECLEVEL_L;
static QRencodeMode hint = QR_MODE_8;
static unsigned char fg_color[4] = {0, 0, 0, 255};
static unsigned char bg_color[4] = {255, 255, 255, 255};

static int verbose = 0;

enum imageType {
	ANSI_TYPE,
	ANSI256_TYPE,
	ASCII_TYPE,
	ASCIIi_TYPE,
	UTF8_TYPE,
	ANSIUTF8_TYPE,
	ANSI256UTF8_TYPE,
	UTF8i_TYPE,
	ANSIUTF8i_TYPE
};

static enum imageType image_type = UTF8_TYPE;

static int color_set(unsigned char color[4], const char *value)
{
	int len = strlen(value);
	int i, count;
	unsigned int col[4];
	if(len == 6) {
		count = sscanf(value, "%02x%02x%02x%n", &col[0], &col[1], &col[2], &len);
		if(count < 3 || len != 6) {
			return -1;
		}
		for(i = 0; i < 3; i++) {
			color[i] = col[i];
		}
		color[3] = 255;
	} else if(len == 8) {
		count = sscanf(value, "%02x%02x%02x%02x%n", &col[0], &col[1], &col[2], &col[3], &len);
		if(count < 4 || len != 8) {
			return -1;
		}
		for(i = 0; i < 4; i++) {
			color[i] = col[i];
		}
	} else {
		return -1;
	}
	return 0;
}

#define MAX_DATA_SIZE (7090 * 2) /* timed by the safty factor 2 */
static unsigned char data_buffer[MAX_DATA_SIZE];
static unsigned char *readFile(FILE *fp, int *length)
{
	int ret;

	ret = fread(data_buffer, 1, MAX_DATA_SIZE, fp);
	if(ret == 0) {
		fprintf(stderr, "No input data.\n");
		exit(EXIT_FAILURE);
	}
	if(feof(fp) == 0) {
		fprintf(stderr, "Input data is too large.\n");
		exit(EXIT_FAILURE);
	}

	data_buffer[ret] = '\0';
	*length = ret;

	return data_buffer;
}

static FILE *openFile(const char *outfile)
{
	FILE *fp;

	if(outfile == NULL || (outfile[0] == '-' && outfile[1] == '\0')) {
		fp = stdout;
	} else {
		fp = fopen(outfile, "wb");
		if(fp == NULL) {
			fprintf(stderr, "Failed to create file: %s\n", outfile);
			perror(NULL);
			exit(EXIT_FAILURE);
		}
	}

	return fp;
}

static void writeANSI_margin(FILE* fp, int realwidth,
                             char* buffer, const char* white, int white_s )
{
	int y;

	strncpy(buffer, white, (size_t)white_s);
	memset(buffer + white_s, ' ', (size_t)realwidth * 2);
	strcpy(buffer + white_s + realwidth * 2, "\033[0m\n"); // reset to default colors
	for(y = 0; y < margin; y++ ){
		fputs(buffer, fp);
	}
}

static int writeANSI(const QRcode *qrcode, const char *outfile)
{
	FILE *fp;
	unsigned char *row, *p;
	int x, y;
	int realwidth;
	int last;

	const char *white, *black;
	char *buffer;
	int white_s, black_s, buffer_s;

	if(image_type == ANSI256_TYPE){
		/* codes for 256 color compatible terminals */
		white = "\033[48;5;231m";
		white_s = 11;
		black = "\033[48;5;16m";
		black_s = 10;
	} else {
		white = "\033[47m";
		white_s = 5;
		black = "\033[40m";
		black_s = 5;
	}

	size = 1;

	fp = openFile(outfile);

	realwidth = (qrcode->width + margin * 2) * size;
	buffer_s = (realwidth * white_s) * 2;
	buffer = (char *)malloc((size_t)buffer_s);
	if(buffer == NULL) {
		fprintf(stderr, "Failed to allocate memory.\n");
		exit(EXIT_FAILURE);
	}

	/* top margin */
	writeANSI_margin(fp, realwidth, buffer, white, white_s);

	/* data */
	p = qrcode->data;
	for(y = 0; y < qrcode->width; y++) {
		row = (p+(y*qrcode->width));

		memset(buffer, 0, (size_t)buffer_s);
		strncpy(buffer, white, (size_t)white_s);
		for(x = 0; x < margin; x++ ){
			strncat(buffer, "  ", 2);
		}
		last = 0;

		for(x = 0; x < qrcode->width; x++) {
			if(*(row+x)&0x1) {
				if( last != 1 ){
					strncat(buffer, black, (size_t)black_s);
					last = 1;
				}
			} else if( last != 0 ){
				strncat(buffer, white, (size_t)white_s);
				last = 0;
			}
			strncat(buffer, "  ", 2);
		}

		if( last != 0 ){
			strncat(buffer, white, (size_t)white_s);
		}
		for(x = 0; x < margin; x++ ){
			strncat(buffer, "  ", 2);
		}
		strncat(buffer, "\033[0m\n", 5);
		fputs(buffer, fp);
	}

	/* bottom margin */
	writeANSI_margin(fp, realwidth, buffer, white, white_s);

	fclose(fp);
	free(buffer);

	return 0;
}

static void writeUTF8_margin(FILE* fp, int realwidth, const char* white,
                             const char *reset, const char* full)
{
	int x, y;

	for (y = 0; y < margin/2; y++) {
		fputs(white, fp);
		for (x = 0; x < realwidth; x++)
			fputs(full, fp);
		fputs(reset, fp);
		fputc('\n', fp);
	}
}

static int writeUTF8(const QRcode *qrcode, const char *outfile, int use_ansi, int invert)
{
	FILE *fp;
	int x, y;
	int realwidth;
	const char *white, *reset;
	const char *empty, *lowhalf, *uphalf, *full;

	empty = " ";
	lowhalf = "\342\226\204";
	uphalf = "\342\226\200";
	full = "\342\226\210";

	if (invert) {
		const char *tmp;

		tmp = empty;
		empty = full;
		full = tmp;

		tmp = lowhalf;
		lowhalf = uphalf;
		uphalf = tmp;
	}

	if (use_ansi){
		if (use_ansi == 2) {
			white = "\033[38;5;231m\033[48;5;16m";
		} else {
			white = "\033[40;37;1m";
		}
		reset = "\033[0m";
	} else {
		white = "";
		reset = "";
	}

	fp = openFile(outfile);

	realwidth = (qrcode->width + margin * 2);

	/* top margin */
	writeUTF8_margin(fp, realwidth, white, reset, full);

	/* data */
	for(y = 0; y < qrcode->width; y += 2) {
		unsigned char *row1, *row2;
		row1 = qrcode->data + y*qrcode->width;
		row2 = row1 + qrcode->width;

		fputs(white, fp);

		for (x = 0; x < margin; x++) {
			fputs(full, fp);
		}

		for (x = 0; x < qrcode->width; x++) {
			if(row1[x] & 1) {
				if(y < qrcode->width - 1 && row2[x] & 1) {
					fputs(empty, fp);
				} else {
					fputs(lowhalf, fp);
				}
			} else if(y < qrcode->width - 1 && row2[x] & 1) {
				fputs(uphalf, fp);
			} else {
				fputs(full, fp);
			}
		}

		for (x = 0; x < margin; x++)
			fputs(full, fp);

		fputs(reset, fp);
		fputc('\n', fp);
	}

	/* bottom margin */
	writeUTF8_margin(fp, realwidth, white, reset, full);

	fclose(fp);

	return 0;
}

static void writeASCII_margin(FILE* fp, int realwidth, char* buffer, int invert)
{
	int y, h;

	h = margin;

	memset(buffer, (invert?'#':' '), (size_t)realwidth);
	buffer[realwidth] = '\n';
	buffer[realwidth + 1] = '\0';
	for(y = 0; y < h; y++ ){
		fputs(buffer, fp);
	}
}

static int writeASCII(const QRcode *qrcode, const char *outfile, int invert)
{
	FILE *fp;
	unsigned char *row;
	int x, y;
	int realwidth;
	char *buffer, *p;
	int buffer_s;
	char black = '#';
	char white = ' ';

	if(invert) {
		black = ' ';
		white = '#';
	}

	size = 1;

	fp = openFile(outfile);

	realwidth = (qrcode->width + margin * 2) * 2;
	buffer_s = realwidth + 2;
	buffer = (char *)malloc((size_t)buffer_s);
	if(buffer == NULL) {
		fprintf(stderr, "Failed to allocate memory.\n");
		exit(EXIT_FAILURE);
	}

	/* top margin */
	writeASCII_margin(fp, realwidth, buffer, invert);

	/* data */
	for(y = 0; y < qrcode->width; y++) {
		row = qrcode->data+(y*qrcode->width);
		p = buffer;

		memset(p, white, (size_t)margin * 2);
		p += margin * 2;

		for(x = 0; x < qrcode->width; x++) {
			if(row[x]&0x1) {
				*p++ = black;
				*p++ = black;
			} else {
				*p++ = white;
				*p++ = white;
			}
		}

		memset(p, white, (size_t)margin * 2);
		p += margin * 2;
		*p++ = '\n';
		*p++ = '\0';
		fputs( buffer, fp );
	}

	/* bottom margin */
	writeASCII_margin(fp, realwidth, buffer, invert);

	fclose(fp);
	free(buffer);

	return 0;
}

static QRcode *encode(const unsigned char *intext, int length)
{
	QRcode *code;

	if(micro) {
		if(eightbit) {
			code = QRcode_encodeDataMQR(length, intext, version, level);
		} else {
			code = QRcode_encodeStringMQR((char *)intext, version, level, hint, casesensitive);
		}
	} else if(eightbit) {
		code = QRcode_encodeData(length, intext, version, level);
	} else {
		code = QRcode_encodeString((char *)intext, version, level, hint, casesensitive);
	}

	return code;
}

static void qrencode(const unsigned char *intext, int length, const char *outfile)
{
	QRcode *qrcode;

	qrcode = encode(intext, length);
	if(qrcode == NULL) {
		if(errno == ERANGE) {
			fprintf(stderr, "Failed to encode the input data: Input data too large\n");
		} else {
			perror("Failed to encode the input data");
		}
		exit(EXIT_FAILURE);
	}

	if(verbose) {
		fprintf(stderr, "File: %s, Version: %d\n", (outfile!=NULL)?outfile:"(stdout)", qrcode->version);
	}

	switch(image_type) {
		case ANSI_TYPE:
		case ANSI256_TYPE:
			writeANSI(qrcode, outfile);
			break;
		case ASCIIi_TYPE:
			writeASCII(qrcode, outfile,  1);
			break;
		case ASCII_TYPE:
			writeASCII(qrcode, outfile,  0);
			break;
		case UTF8_TYPE:
			writeUTF8(qrcode, outfile, 0, 0);
			break;
		case ANSIUTF8_TYPE:
			writeUTF8(qrcode, outfile, 1, 0);
			break;
		case ANSI256UTF8_TYPE:
			writeUTF8(qrcode, outfile, 2, 0);
			break;
		case UTF8i_TYPE:
			writeUTF8(qrcode, outfile, 0, 1);
			break;
		case ANSIUTF8i_TYPE:
			writeUTF8(qrcode, outfile, 1, 1);
			break;
		default:
			fprintf(stderr, "Unknown image type.\n");
			exit(EXIT_FAILURE);
	}

	QRcode_free(qrcode);
}

static QRcode_List *encodeStructured(const unsigned char *intext, int length)
{
	QRcode_List *list;

	if(eightbit) {
		list = QRcode_encodeDataStructured(length, intext, version, level);
	} else {
		list = QRcode_encodeStringStructured((char *)intext, version, level, hint, casesensitive);
	}

	return list;
}

static void qrencodeStructured(const unsigned char *intext, int length, const char *outfile)
{
	QRcode_List *qrlist, *p;
	char filename[FILENAME_MAX];
	char *base, *q, *suffix = NULL;
	const char *type_suffix;
	int i = 1;
	size_t suffix_size;

	switch(image_type) {
		case ANSI_TYPE:
		case ANSI256_TYPE:
		case ASCII_TYPE:
		case UTF8_TYPE:
		case ANSIUTF8_TYPE:
		case UTF8i_TYPE:
		case ANSIUTF8i_TYPE:
			type_suffix = ".txt";
			break;
		default:
			fprintf(stderr, "Unknown image type.\n");
			exit(EXIT_FAILURE);
	}

	if(outfile == NULL) {
		fprintf(stderr, "An output filename must be specified to store the structured images.\n");
		exit(EXIT_FAILURE);
	}
	base = strdup(outfile);
	if(base == NULL) {
		fprintf(stderr, "Failed to allocate memory.\n");
		exit(EXIT_FAILURE);
	}
	suffix_size = strlen(type_suffix);
	if(strlen(base) > suffix_size) {
		q = base + strlen(base) - suffix_size;
		if(strcasecmp(type_suffix, q) == 0) {
			suffix = strdup(q);
			*q = '\0';
		}
	}

	qrlist = encodeStructured(intext, length);
	if(qrlist == NULL) {
		if(errno == ERANGE) {
			fprintf(stderr, "Failed to encode the input data: Input data too large\n");
		} else {
			perror("Failed to encode the input data");
		}
		exit(EXIT_FAILURE);
	}

	for(p = qrlist; p != NULL; p = p->next) {
		if(p->code == NULL) {
			fprintf(stderr, "Failed to encode the input data.\n");
			exit(EXIT_FAILURE);
		}
		if(suffix) {
			snprintf(filename, FILENAME_MAX, "%s-%02d%s", base, i, suffix);
		} else {
			snprintf(filename, FILENAME_MAX, "%s-%02d", base, i);
		}

		if(verbose) {
			fprintf(stderr, "File: %s, Version: %d\n", filename, p->code->version);
		}

		switch(image_type) {
			case ANSI_TYPE:
			case ANSI256_TYPE:
				writeANSI(p->code, filename);
				break;
			case ASCIIi_TYPE:
				writeASCII(p->code, filename, 1);
				break;
			case ASCII_TYPE:
				writeASCII(p->code, filename, 0);
				break;
			case UTF8_TYPE:
				writeUTF8(p->code, filename, 0, 0);
				break;
			case ANSIUTF8_TYPE:
				writeUTF8(p->code, filename, 0, 0);
				break;
			case ANSI256UTF8_TYPE:
				writeUTF8(p->code, filename, 0, 0);
				break;
			case UTF8i_TYPE:
				writeUTF8(p->code, filename, 0, 1);
				break;
			case ANSIUTF8i_TYPE:
				writeUTF8(p->code, filename, 0, 1);
				break;

			default:
				fprintf(stderr, "Unknown image type.\n");
				exit(EXIT_FAILURE);
		}
		i++;
	}

	free(base);
	if(suffix) {
		free(suffix);
	}

	QRcode_List_free(qrlist);
}

int main(int argc, char **argv)
{
	int opt, lindex = -1;
	char *outfile = "/dev/stdout";
	unsigned char *intext = "AAAAAAAAAAAAAAAAAAA";
	int length = 0;

	if(margin < 0) {
		if(micro) {
			margin = 2;
		} else {
			margin = 4;
		}
	}

	if(structured) {
		if(version == 0) {
			fprintf(stderr, "Version must be specified to encode structured symbols.\n");
			exit(EXIT_FAILURE);
		}
		qrencodeStructured(intext, length, outfile);
	} else {
		qrencode(intext, length, outfile);
	}

	return 0;
}
