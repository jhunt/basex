#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

#include "base16.h"
#include "base32.h"
#include "base64.h"

typedef int(*decode)(char *, const char *, size_t);
typedef size_t(*declen)(size_t);
typedef void(decalph)(const char *);

static inline void bxa(int base, decalph fn, const char *alpha)
{
	if (strlen(alpha) != base) {
		fprintf(stderr, "bxd: desired alphabet '%s' is not %d characters in length.\n", alpha, base);
		exit(3);
	}
	fn(alpha);
}

#define DECBUFSIZ 4096
#define INBUFSIZ DECBUFSIZ*2
int main(int argc, char **argv)
{
	size_t n;
	char in[INBUFSIZ];
	char buf[DECBUFSIZ];
	decode bxd;
	declen bxdlen;

	memset(in, 0, INBUFSIZ);

	if (argc < 2 || argc > 3) {
		fprintf(stderr, "USAGE: bxd <file BASE [ALPHABET]\n");
		return 1;
	}

	if (strcmp(argv[1], "16") == 0) {
		if (argc == 3) bxa(16, b16a, argv[2]);
		bxd = b16d; bxdlen = b16dlen;

	} else if (strcmp(argv[1], "32") == 0) {
		if (argc == 3) bxa(32, b32a, argv[2]);
		bxd = b32d; bxdlen = b32dlen;

	} else if (strcmp(argv[1], "64") == 0) {
		if (argc == 3) bxa(64, b64a, argv[2]);
		bxd = b64d; bxdlen = b64dlen;

	} else {
		fprintf(stderr, "bxd: unrecognized base '%s'\n", argv[1]);
		return 2;
	}

	while ((n = read(0, in, INBUFSIZ)) > 0) {
		bxd(buf, in, n);
		write(1, buf, bxdlen(n));
	}
	return 0;
}
