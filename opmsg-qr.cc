/*
 * This file is part of the opmsg crypto message framework.
 *
 * (C) 2019 by Sebastian Krahmer,
 *             sebastian [dot] krahmer [at] gmail [dot] com
 *
 * opmsg is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * opmsg is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with opmsg.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <string>
#include <memory>
#include <unistd.h>
#include <getopt.h>
#include "qrenc.h"
#include "keystore.h"
#include "config.h"
#include "misc.h"


using namespace std;
using namespace opmsg;


extern "C" int quirc_scan(const char *, int, int, char **, size_t *);


int get_persona(const string &hexid, string &pub_pem)
{
	pub_pem = "";
	if (hexid.size() < 16 || !is_hex_hash(hexid))
		return -1;

	keystore ks(config::phash, config::cfgbase);
	if (ks.load(hexid, LFLAGS_ALL & ~LFLAGS_KEX) < 0)
		return -1;

	persona *p = ks.find_persona(hexid);

	if (!p || p->load("", LFLAGS_ALL & ~LFLAGS_KEX) < 0)
		return -1;

	pub_pem = p->get_pkey()->get_pub_pem();

	printf("\n%s %s\n\n", p->get_id().c_str(), p->get_name().c_str());

	return 0;
}


int do_import(const string &camera, int dry, const string &name)
{
	char *result = nullptr;
	size_t reslen = 0;

	printf("\nImporting with name %s and phash %s\n", name.c_str(), config::phash.c_str());
	printf("Put QR Code in front of camera %s\n", camera.c_str());

	if (quirc_scan(camera.c_str(), 1280, 720, &result, &reslen) < 0)
		return -1;

	string pem = string(result, reslen);
	free(result);

	printf("\n%s\n", pem.c_str());

	if (dry)
		return 0;

	if (pem.find(marker::pub_begin) != 0 || pem.find(marker::pub_end) + marker::pub_end.size() + 1 != pem.size()) {
		fprintf(stderr, "Invalid pubkey. Not importing.\n");
		return -1;
	}

	keystore ks(config::phash, config::cfgbase);

	persona *p = nullptr;
	if (!(p = ks.add_persona(name, pem, "", ""))) {
		fprintf(stderr, "%s\n", ks.why());
		return -1;
	}

	printf("Imported persona with id %s\n", p->get_id().c_str());

	return 0;
}


void usage(const char *p)
{
	exit(1);
}


int main(int argc, char **argv)
{
	const char *outfile = "/dev/stdout";
	string hexid = "", pub_pem = "", camera = "/dev/video0", import = "";
	struct option lopts[] = {
	        {"confdir", required_argument, nullptr, 'c'},
		{"help", no_argument, nullptr, 'h'},
	        {"qr", required_argument, nullptr, 'q'},
	        {"import", required_argument, nullptr, 'i'},
	        {"nopem", no_argument, nullptr, 'P'},
	        {"camera", required_argument, nullptr, 'C'},
	        {"dry", no_argument, nullptr, 'd'},
	        {nullptr, 0, nullptr, 0}};
	int c = 0, opt_idx = 0, dump_pem = 1, dry = 0;

	if (getenv("HOME")) {
		config::cfgbase = getenv("HOME");
		config::cfgbase += "/.opmsg";
	}

	if (argc == 1)
		usage(argv[0]);

	if (strcmp(argv[1], "-c") == 0 || strcmp(argv[1], "--confdir") == 0) {
		if (!argv[2])
			usage(argv[0]);
		if (strcmp(argv[2], "") != 0)
			config::cfgbase = argv[2];
	}

	if (parse_config(config::cfgbase) < 0) {
		fprintf(stderr, "Error parsing config file.\n");
		return -1;
	}

	while ((c = getopt_long(argc, argv, "c:hq:i:PC:d", lopts, &opt_idx)) != -1) {
		switch (c) {
		case 'c':
			// already handled
			break;
		case 'q':
			hexid = optarg;
			break;
		case 'P':
			dump_pem = 0;
			break;
		case 'i':
			import = optarg;
			break;
		case 'C':
			camera = optarg;
			break;
		case 'd':
			dry = 1;
			break;
		case 'h':
		default:
			usage(argv[0]);
			break;
		}
	}

	if (hexid.size() > 0) {
		if (get_persona(hexid, pub_pem)) {
			fprintf(stderr, "Failed to get persona with id %s\n", hexid.c_str());
			return -1;
		}

		if (dump_pem)
			printf("%s\n\n",  pub_pem.c_str());

		qrencode(reinterpret_cast<const unsigned char *>(pub_pem.c_str()), pub_pem.size(), outfile, UTF8_TYPE);
		printf("\n");

	} else if (import.size() > 0) {
		if (do_import(camera, dry, import) < 0) {
			fprintf(stderr, "Failed to import QR code.\n");
			return -1;
		}
	}

	return 0;
}

