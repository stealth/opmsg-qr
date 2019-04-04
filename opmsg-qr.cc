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


int get_persona(const string &hexid, string &pub_pem)
{
	pub_pem = "";
	if (hexid.size() < 16 || !is_hex_hash(hexid))
		return -1;

	keystore ks(config::phash, config::cfgbase);
	if (ks.load(hexid, LFLAGS_ALL & ~LFLAGS_KEX) < 0)
		return -1;

	persona *p = ks.find_persona(hexid);

	if (!p || p->load("", LFLAGS_ALL & ~LFLAGS_KEX) < 0) {
		return -1;
	}

	pub_pem = p->get_pkey()->get_pub_pem();

	printf("\n%s %s\n\n", p->get_id().c_str(), p->get_name().c_str());

	return 0;
}



int main(int argc, char **argv)
{
	string hexid = "", pub_pem = "";
	struct option lopts[] = {
	        {"confdir", required_argument, nullptr, 'c'},
		{"help", no_argument, nullptr, 'h'},
	        {"show", required_argument, nullptr, 's'},
	        {nullptr, 0, nullptr, 0}};
	int c = 0, opt_idx = 0;

	if (getenv("HOME")) {
		config::cfgbase = getenv("HOME");
		config::cfgbase += "/.opmsg";
	}

	if (parse_config(config::cfgbase) < 0) {
		printf("cccc\n");
		return -1;
	}

	while ((c = getopt_long(argc, argv, "hs:", lopts, &opt_idx)) != -1) {
		switch (c) {
		case 'h':
			break;
		case 's':
			hexid = optarg;
			break;
		}
	}

	const char *outfile = "/dev/stdout";

	get_persona(hexid, pub_pem);

	qrencode(reinterpret_cast<const unsigned char *>(pub_pem.c_str()), pub_pem.size(), outfile, UTF8_TYPE);

	printf("\n");
	return 0;
}

