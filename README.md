opmsg-qr
========

`opmsg-qr` is a convenience tool which allows to dump and import opmsg
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

If you already cloned the `opmsg` repo before, do not forget to `git pull`
and `make -C src clean` as the initial step, instead of the `git clone ...`.

Run
---

You should run it only on a UTF8 terminal.

```
$ opmsg-qr

opmsg-qr v0.21 (C) 2020 Sebastian Krahmer: https://github.com/stealth/opmsg-qr


Usage: opmsg-qr [--confdir dir] [--help] [--qr hexid] [--import name] [--nopem]
		[--camera device] [--dry] [--invert] [--phash algo] [-x width] [-y height]

	--confdir,	-c	(must come first) defaults to ~/.opmsg
	--help,		-h	this help
	--qr, 		-q	show QR code of this persona id
	--import,	-i	import QR code as persona with this name
	--nopem,	-P	do not print PEM key, just QR code
	--width,	-x	camera resolution (640)
	--height,	-y	camera resolution (480)
	--camera,	-C	use this camera device (defaults to /dev/video0)
	--dry,		-d	don't actually import, just decode and show PEM key
	--invert,	-I	invert black/white for terminals with white background color
	--phash,	-p	use this persona hash algo (defaults to sha256)


$ opmsg-qr -q a6da74f688c375d8

opmsg-qr v0.21 (C) 2020 Sebastian Krahmer: https://github.com/stealth/opmsg-qr


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

You should use a camera with auto-focus. Not using auto-focus makes it very
painful to impossible to capture the QR code from the phone screen. Do not make
the picture with your phone too close to the screen. It's better to have the QR
code smaller with the possibility to zoom, instead of having the
border on the edged screen.

While importing, `opmsg-qr` will try to help you finding the right distance
and angle for the camera. It prints `?` if it can't detect anything. If you
see '!', there is already something that the QR-reader can detect. In that case
you need to keep the camera/phone stable or only slightly adjust it.

*Important:* The parameters for dumping the QR-code on the screen are set to black/dark
UTF8 terminals like *xterm* (for example with black or blue background color).
If you are using white background, you have to `--invert` the printing,
in order to have the QR blocks properly colored for detection.

The `-x` and `-y` parameters are very likely not necessary to be changed.

After all, webcams have usually less resolution and quality than high end
smartphone cams, so the user experience when scanning QR codes with webcams,
is somewhat different than with smartphone apps. You can test your cam
at the console with vlc like `vlc v4l2:///dev/video0` and check how fast/slow
your autofocus is.


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


