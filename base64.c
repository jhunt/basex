#include <stddef.h>
#include <string.h>

#include "base64.h"

#define MASK 0x3f

static char ALPHA[64] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
static char LOOKUP[256] = {
	   0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
	   0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
	   0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0, 0x3e,    0,    0,    0, 0x3f,
	0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x3a, 0x3b, 0x3c, 0x3d,    0,    0,    0,    0,    0,    0,
	   0,    0, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e,
	0x0f, 0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19,    0,    0,    0,    0,    0,
	   0, 0x1a, 0x1b, 0x1c, 0x1d, 0x1e, 0x1f, 0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28,
	0x29, 0x2a, 0x2b, 0x2c, 0x2d, 0x2e, 0x2f, 0x30, 0x31, 0x32, 0x33,    0,    0,    0,    0,    0,
	   0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
	   0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
	   0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
	   0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
	   0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
	   0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
	   0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
	   0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
};

void b64a(const char *alpha) {
	const char *c;
	memset(LOOKUP, 0, sizeof(LOOKUP));
	for (c = alpha; *c; c++) {
		ALPHA[c-alpha] = *c;
		LOOKUP[*c] = c-alpha;
	}
}

/*

          [0]      [1]      [2]
   8b: 01234567 01234567 01234567

   5b: 012345 670123 456701 234567
          ^      ^      ^      ^
          |      |      |      `-- (           [2]     ) & 0x3f
          |      |      `--------- ([1] << 2 | [2] >> 6) & 0x3f
          |      `---------------- ([0] << 4 | [1] >> 4) & 0x3f
          `----------------------- (           [0] >> 2) & 0x3f

    01234567
    012345--   [0] >> 2

    01234567 01234567
    ------67 0123----  [0] << 4 | [1] >> 4

             01234567 01234567
             ----4567 01------  [1] << 2 | [2] >> 6

                      01234567
                      --234567  [2]


    012345 670123 456701 234567
    012345 670123
    012345 67----  [0] << 2 | [1] >> 4

           670123 456701
           --0123 4567--  [1] << 4 | [2] >> 2

                  456701 234567
                  ----01 234567  [2] << 6 | [3]

 */


int
b64e(char *dst, const char *src, size_t inlen)
{
	char buf[3];
	for (; inlen >= 3; src += 3, inlen -= 3) {
		*dst++ = ALPHA[ (                 src[0] >> 2 ) & MASK];
		*dst++ = ALPHA[ ((src[0] << 4) | (src[1] >> 4)) & MASK];
		*dst++ = ALPHA[ ((src[1] << 2) | (src[2] >> 6)) & MASK];
		*dst++ = ALPHA[ (                 src[2]      ) & MASK];
	}

	if (inlen >= 1) {
		/* bounce through a temporary (stack-local)
		   buffer for our required 0-padding. */
		memset(buf, 0, 4);
		memcpy(buf, src, inlen);

		*dst++ = ALPHA[ (                 src[0] >> 2 ) & MASK];
		*dst++ = ALPHA[ ((src[0] << 4) | (src[1] >> 4)) & MASK];
	}
	if (inlen >= 2) {
		*dst++ = ALPHA[ ((src[1] << 2) | (src[2] >> 6)) & MASK];
	}
	if (inlen >= 3) {
		*dst++ = ALPHA[ (                 src[2]      ) & MASK];
	}

	*dst = '\0';
	return 0;
}

int
b64d(char *dst, const char *src, size_t inlen)
{
	if (inlen % 4 == 1)
		return 1;

	for (; inlen >= 4; src += 4, inlen -= 4) {
		*dst++ = (LOOKUP[src[0]] << 2) | (LOOKUP[src[1]] >> 4);
		*dst++ = (LOOKUP[src[1]] << 4) | (LOOKUP[src[2]] >> 2);
		*dst++ = (LOOKUP[src[2]] << 6) | (LOOKUP[src[3]]     );
	}
	if (inlen >= 2) *dst++ = (LOOKUP[src[0]] << 2) | (LOOKUP[src[1]] >> 4);
	if (inlen >= 3) *dst++ = (LOOKUP[src[1]] << 4) | (LOOKUP[src[2]] >> 2);

	*dst = '\0';
	return 0;
}

#ifdef O_TESTS
#include "ctap.h"

#define b64_is(buf, in, out) do {\
	memset(buf, 0, sizeof(buf)); \
	ok(b64e(buf, in, strlen(in)) == 0, "b64e(" in ") should succeed"); \
	is(buf, out, "[" in "] is [" out "] in base64"); \
	memset(buf, 0, sizeof(buf)); \
	ok(b64d(buf, out, strlen(out)) == 0, "b64d(" out ") should succeed"); \
	is(buf, in, "[" out "] in base64 is [" in "]"); \
} while (0)

#define b64_noop(buf, s) do {\
	memset(buf, 0, sizeof(buf)); \
	ok(b64e(buf, s, strlen(s)) == 0, "b64e(" s ") should succeed"); \
	ok(b64d(buf, buf, strlen(buf)) == 0, "b64d(b64e(" s ")) should also succeed"); \
	is(buf, s, "D(E(s)) should equal (s)"); \
} while (0)

TESTS {
	char buf[256];

	b64_is(buf, "f",      "Zg");
	b64_is(buf, "fo",     "Zm8");
	b64_is(buf, "foo",    "Zm9v");
	b64_is(buf, "foob",   "Zm9vYg");
	b64_is(buf, "fooba",  "Zm9vYmE");
	b64_is(buf, "foobar", "Zm9vYmFy");

	b64_noop(buf, "people say nothing is impossible, but i do nothing every day.");
	b64_noop(buf, "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789");
}
#endif
