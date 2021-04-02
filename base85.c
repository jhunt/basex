#include <stddef.h>
#include <string.h>
#include <stdint.h>
#include <stdio.h>

#include "base85.h"

static char ALPHA[85] = "!\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstu";
static char LOOKUP[256] = {
	   0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
	   0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
	   0,    0, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e,
	0x0f, 0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1a, 0x1b, 0x1c, 0x1d, 0x1e,
	0x1f, 0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28, 0x29, 0x2a, 0x2b, 0x2c, 0x2d, 0x2e,
	0x2f, 0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x3a, 0x3b, 0x3c, 0x3d, 0x3e,
	0x3f, 0x40, 0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48, 0x49, 0x4a, 0x4b, 0x4c, 0x4d, 0x4e,
	0x4f, 0x50, 0x51, 0x52, 0x53, 0x54,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
	   0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
	   0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
	   0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
	   0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
	   0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
	   0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
	   0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
	   0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
};

void b85a(const char *alpha) {
	const char *c;
	memset(LOOKUP, 0, sizeof(LOOKUP));
	for (c = alpha; *c; c++) {
		ALPHA[c-alpha] = *c;
		LOOKUP[*c] = c-alpha;
	}
}

int
b85e(char *dst, const char *src, size_t inlen)
{
	uint32_t acc;
	char buf[4];

	for (; inlen >= 4; src += 4, inlen -= 4) {
		acc = src[0] << 24
		    | src[1] << 16
		    | src[2] <<  8
		    | src[3];

		*dst++ = ALPHA[ acc / 85 / 85 / 85 / 85 % 85];
		*dst++ = ALPHA[ acc / 85 / 85 / 85 % 85     ];
		*dst++ = ALPHA[ acc / 85 / 85 % 85          ];
		*dst++ = ALPHA[ acc / 85 % 85               ];
		*dst++ = ALPHA[ acc % 85                    ];
	}

	if (inlen >= 1) {
		/* bounce through a temporary (stack-local)
		   buffer for our required 0-padding. */
		memset(buf, 0, 4);
		memcpy(buf, src, inlen);

		acc = buf[0] << 24
		    | buf[1] << 16
		    | buf[2] <<  8
		    | buf[4];

		*dst++ = ALPHA[ acc / 85 / 85 / 85 / 85 % 85];
		*dst++ = ALPHA[ acc / 85 / 85 / 85 % 85     ];
	}
	if (inlen >= 2) {
		*dst++ = ALPHA[ acc / 85 / 85 % 85          ];
	}
	if (inlen >= 3) {
		*dst++ = ALPHA[ acc / 85 % 85               ];
	}
	return 0;
}

int
b85d(char *dst, const char *src, size_t inlen)
{
	uint32_t acc;
	char buf[5];

	for (; inlen >= 5; src += 5, inlen -= 5) {
		acc = LOOKUP[src[0]] * 85 * 85 * 85 * 85
		    + LOOKUP[src[1]] * 85 * 85 * 85
		    + LOOKUP[src[2]] * 85 * 85
		    + LOOKUP[src[3]] * 85
		    + LOOKUP[src[4]];

		*dst++ = (acc >> 24) & 0xff;
		*dst++ = (acc >> 16) & 0xff;
		*dst++ = (acc >>  8) & 0xff;
		*dst++ = (acc)       & 0xff;
	}

	if (inlen >= 1) {
		memset(buf, 'u', 5);
		memcpy(buf, src, inlen);

		acc = LOOKUP[buf[0]] * 85 * 85 * 85 * 85
		    + LOOKUP[buf[1]] * 85 * 85 * 85
		    + LOOKUP[buf[2]] * 85 * 85
		    + LOOKUP[buf[3]] * 85
		    + LOOKUP[buf[4]];
	}
	if (inlen >= 2) *dst++ = (acc >> 24) & 0xff;
	if (inlen >= 3) *dst++ = (acc >> 16) & 0xff;
	if (inlen >= 4) *dst++ = (acc >>  8) & 0xff;
	return 0;
}

#ifdef O_TESTS
#include "ctap.h"

#define b85_is(buf, in, out) do {\
	memset(buf, 0, sizeof(buf)); \
	ok(b85e(buf, in, strlen(in)) == 0, "b85e(" in ") should succeed"); \
	buf[b85elen(strlen(in))] = '\0'; \
	is(buf, out, "[" in "] is [" out "] in base85"); \
	\
	memset(buf, 0, sizeof(buf)); \
	ok(b85d(buf, out, strlen(out)) == 0, "b85d(" out ") should succeed"); \
	buf[b85elen(strlen(in))] = '\0'; \
	is(buf, in, "[" out "] in base85 is [" in "]"); \
} while (0)

#define b85_uses(e,d) do {\
	cmp_ok(b85elen(e), "==", d, "base-85 needs %d bytes to encode %d bytes", e, d); \
	cmp_ok(b85dlen(d), "==", e, "base-85 needs %d bytes to decode %d bytes", d, e); \
} while (0)

#define b85_noop(buf, s) do {\
	memset(buf, 0, sizeof(buf)); \
	ok(b85e(buf, s, strlen(s)) == 0, "b85e(" s ") should succeed"); \
	ok(b85d(buf, buf, strlen(buf)) == 0, "b85d(b85e(" s ")) should also succeed"); \
	buf[b85dlen(b85elen(strlen(s)))] = '\0'; \
	is(buf, s, "D(E(s)) should equal (s)"); \
} while (0)

TESTS {
	char buf[256];

	b85_uses(1,2); b85_uses(2,3); b85_uses(3,4); b85_uses(4,5);
	b85_uses(5,7); /* and we build! */

	b85_is(buf, "f",      "Ac");
	b85_is(buf, "fo",     "Ao@");
	b85_is(buf, "foo",    "AoDS");
	b85_is(buf, "foob",   "AoDTs");
	b85_is(buf, "fooba",  "AoDTs@/");
	b85_is(buf, "foobar", "AoDTs@<)");

	b85_noop(buf, "people say nothing is impossible, but i do nothing every day.");
	b85_noop(buf, "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789");
}
#endif
