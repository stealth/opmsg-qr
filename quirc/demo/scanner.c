/* quirc -- QR-code recognition library
 * Copyright (C) 2010-2012 Daniel Beer <dlbeer@gmail.com>
 *
 * Permission to use, copy, modify, and/or distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#include <stdlib.h>
#include <ctype.h>
#include <unistd.h>
#include <quirc.h>
#include <string.h>
#include <time.h>
#include <getopt.h>

#include "camera.h"
#include "mjpeg.h"
#include "convert.h"
#include "dthash.h"

static int printer_timeout = 1;

static int main_loop(struct camera *cam,
		     struct quirc *q, struct mjpeg_decoder *mj, char **result, size_t *reslen)
{
	struct dthash dt;

	dthash_init(&dt, printer_timeout);

	for (;;) {
		int w, h;
		int i, count;
		uint8_t *buf = quirc_begin(q, &w, &h);
		const struct camera_buffer *head;
		const struct camera_parms *parms = camera_get_parms(cam);

		if (camera_dequeue_one(cam) < 0) {
			perror("camera_dequeue_one");
			return -1;
		}

		head = camera_get_head(cam);

		switch (parms->format) {
		case CAMERA_FORMAT_MJPEG:
			mjpeg_decode_gray(mj, head->addr, head->size,
					  buf, w, w, h);
			break;

		case CAMERA_FORMAT_YUYV:
			yuyv_to_luma(head->addr, w * 2, w, h, buf, w);
			break;

		default:
			fprintf(stderr, "Unknown frame format\n");
			return -1;
		}

		if (camera_enqueue_all(cam) < 0) {
			perror("camera_enqueue_all");
			return -1;
		}

		quirc_end(q);

		if ((count = quirc_count(q)) == 0)
			printf("?");
		else if (count == 1)
			printf("!");
		else
			printf("-");

		for (i = 0; i < count; i++) {
			struct quirc_code code;
			struct quirc_data data;

			quirc_extract(q, i, &code);
			if (!quirc_decode(&code, &data)) {
				if (data.payload_len <= 0 || data.payload_len > sizeof(data.payload))
					return -1;
				*result = malloc(data.payload_len);
				if (*result) {
					memcpy(*result, data.payload, data.payload_len);
					*reslen = data.payload_len;
				}
				return 0;
			}
		}
	}
}


int quirc_scan(const char *camera_path, int video_width, int video_height, char **result, size_t *reslen)
{
	struct quirc *qr;
	struct camera cam;
	struct mjpeg_decoder mj;
	const struct camera_parms *parms;

	camera_init(&cam);
	if (camera_open(&cam, camera_path, video_width, video_height,
			25, 1) < 0) {
		perror("camera_open");
		goto fail_qr;
	}

	if (camera_map(&cam, 4) < 0) {
		perror("camera_map");
		goto fail_qr;
	}

	if (camera_on(&cam) < 0) {
		perror("camera_on");
		goto fail_qr;
	}

	if (camera_enqueue_all(&cam) < 0) {
		perror("camera_enqueue_all");
		goto fail_qr;
	}

	parms = camera_get_parms(&cam);

	qr = quirc_new();
	if (!qr) {
		perror("couldn't allocate QR decoder");
		goto fail_qr;
	}

	if (quirc_resize(qr, parms->width, parms->height) < 0) {
		perror("couldn't allocate QR buffer");
		goto fail_qr_resize;
	}

	mjpeg_init(&mj);
	if (main_loop(&cam, qr, &mj, result, reslen) < 0)
		goto fail_main_loop;
	mjpeg_free(&mj);

	quirc_destroy(qr);
	camera_destroy(&cam);

	return 0;

fail_main_loop:
	mjpeg_free(&mj);
fail_qr_resize:
	quirc_destroy(qr);
fail_qr:
	camera_destroy(&cam);

	return -1;
}

