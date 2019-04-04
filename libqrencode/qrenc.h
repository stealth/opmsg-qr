#ifndef QRENC_H
#define QRENC_H

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

#ifdef __cplusplus
extern "C" {
#endif

void qrencode(const unsigned char *intext, int length, const char *outfile, enum imageType);

#ifdef __cplusplus
}
#endif


#endif

