opmsg-qr
========

`opmsg-qr` is a conveniance tool which allows to dump and import opmsg
personas as QR codes. The QR code, representing the public key of this
persona, will be printed on `stdout` as *UTF8*. It may then be snapped with
your smartphone and sent across a *Signal* or other secure channels. The
peer can then import it via `opmsg-qr` by placing the phone-screen in front
of the laptop camera.
If the transport channel of the QR code is considered safe, comparing
the persona ids is optional.

Build
-----

Building `opmsg-qr` requires to clone and build `opmsg` first:

```
$ git clone https://github.com/stealth/opmsg
$ cd opmsg
$ make -C src
[...]
$ git clone https://github.com/stealth/opmsg-qr
$ make -C opmsg-qr
```

The `opmsg-qr` repo has to be cloned inside the `opmsg` directory.
Alternatively you may edit the `$OPMSG` variable inside the `Makefile`.

You will need `libcrypto` from *OpenSSL* or *LibreSSL* and `libjpeg` for a
successful build.

Run
---

You should run it only on a UTF8 terminal.

```
$ opmsg-qr

opmsg-qr v0.2 (C) 2019 Sebastian Krahmer: https://github.com/stealth/opmsg-qr


Usage: opmsg-qr [--confdir dir] [--help] [--qr hexid] [--import name] [--nopem]
		[--camera device] [--dry] [--phash algo]

	--confdir,	-c	(must come first) defaults to ~/.opmsg
	--help,		-h	this help
	--qr, 		-q	show QR code of this persona id
	--import,	-i	import QR code as persona with this name
	--nopem,	-P	do not print PEM key, just QR code
	--camera,	-C	use this camera device (defaults to /dev/video0)
	--dry,		-d	don't actually import, just decode and show PEM key
	--phash,	-p	use this persona hash algo (defaults to sha256)



$ opmsg-qr -q a6da74f688c375d8

opmsg-qr v0.2 (C) 2019 Sebastian Krahmer: https://github.com/stealth/opmsg-qr


a6da74f688c375d896858709ffd1a25fd42e6523bb89d0f245cfe554cf7b4e7c nobrainer

-----BEGIN PUBLIC KEY-----
MFgwEAYHKoZIzj0CAQYFK4EEACMDRAACAduz57cNtmSx4ctIRw6gj6dgHK+DUQHV
TYn4q3gcm7e/PO7gdJ/iq9SzRgDZd/NesHAFhdfdgb7OQZNmHPoJL9+J
-----END PUBLIC KEY-----


█████████████████████████████████████████████
██ ▄▄▄▄▄ █▄█ ▄█▄▄▄ █▀▀▀  ▀▀▀  █▄▄█▀█ ▄▄▄▄▄ ██
██ █   █ █▀▄▄▄▀ ▄▀█ ▄██▀▄ ██▄▀ █▀▀▄█ █   █ ██
██ █▄▄▄█ █ ▄  █▄▄█ ▀▀ ▄█▀▄██▄█▄▀█▀▀█ █▄▄▄█ ██
██▄▄▄▄▄▄▄█ ▀▄█▄█▄█ ▀ █▄▀▄█ █▄▀ ▀▄▀ █▄▄▄▄▄▄▄██
██ ▀▀▀▄█▄▄ █ █▄▀█▄ █ ▄ ▄██ ▀▄▄▀█    ▄▄▄ ▀▀▄██
███▀▀▀▀▄▄ █▀▄█ ▄▄█ ▄▀██  ▄▀▄ ▀ ▄▄██▀█▄▀▀█████
██▄▄██▀█▄▀▀█▄ █   ▄██ ▄▀▀█▀▄ ▄█ ▄ ▀▄█▀█▀▀ ▀██
██ ▄████▄▀ ▀█▀▀  ██ █ ▀ ▄ ▄ ▀███▄▀▀  ▀▄█▄█▀██
███▄  ▀▀▄▄█▀ ██ ▀▀ ▀ █▀▄ █ ▄█ ▀ ▀▄▀ █▄▀ ▄█▄██
██▄▄▄█▄█▄▄ ▄▄███▄▄▄█▄ ▀ ▄▄▀ ▄██▀█▀ █▄▀██▀  ██
██ ▄▄▀▄▄▄   ▄▄▀  █ █▀▀█▀▀▀ ▄▀▄▀█ █▀▀ ▀▄▄█▀▀██
██ █ ███▄ █▄▀█▀ ███▀▀▀▄█▄█ █ ▄▀▀ ▄ ▄█▄ ▄▀▀▄██
██▀▄▄▀█▄▄█▄▀█▀█▀▄ ██▀  ▀█▄ ▄ ▄▀▄ ▀▀▀ ▄ ▄  ▄██
███▀▄▀▄▄▄██ ▄ ▀ █▀▀  █▀▄▀ ▄ ▄▄▄▄▀██▄  ▀ ██▀██
██▀▄█▄ ▄▄  ██▀ ▀█▀██▀ ▀▄██▄▀ █ ▄▀▄▄▀▄▄█ ▀▄███
██████ ▄▄ ▀▀▀▀█ ▀█▄▄ █▄███▀▀▄██ █▀▄ ▀▀  ▀▄▀██
██▄▄████▄█▀█▄ ▀▄ ▄▄█  █  ▄▄▀▄▄▄ █▄ ▄▄▄ ▀▄▄▄██
██ ▄▄▄▄▄ █▄  ▄▄██▄▄ ▀▀▄█▀▄▄█▀▀▄ ▀▄ █▄█  ▄  ██
██ █   █ █▄▀▄   ▀▀▀▀▄  █▄█  ▄█▀  █▄▄▄ ▄▀▀ ███
██ █▄▄▄█ ███▀▀█▀▄█ ▀▀   ▄▀█▄█▀▀██▄██▀ ▀▄▀▀ ██
██▄▄▄▄▄▄▄█▄▄██▄█▄▄▄███▄███▄▄▄███▄▄▄▄▄█▄██▄███
█████████████████████████████████████████████

```


3rd party code
--------------

`opmsg-qr` is using code of the following projects:

* libqrencode (C) by Kentaro Fukuchi
* quirc (C) by Daniel Beer

Both may be found on github. Both are open source, and their license files are
included in this repo inside their subdirs. They implement a small footprint
QR encoding and decoding library in C. I cloned both repos inside `opmsg-qr`
and stripped down all unnecessary parts, re-wrote the build scripts and fixed
some smaller things. I also reviewed the QR decoding part to not contain overflows
and such and to have a secure code base.
I left the original README and license files unchanged. If in doubt, any potential
bugs were made by me during import and not by their original authors.


