#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

#include "base16.h"
#include "base32.h"
#include "base64.h"
#include "base85.h"

typedef int(*encode)(char *, const char *, size_t);
typedef size_t(*enclen)(size_t);
typedef void(encalph)(const char *);

static inline void bxa(int base, encalph fn, const char *alpha)
{
	if (strlen(alpha) != base) {
		fprintf(stderr, "bxe: desired alphabet '%s' is not %d characters in length.\n", alpha, base);
		exit(3);
	}
	fn(alpha);
}

#define INBUFSIZ 4096
#define ENCBUFSIZ INBUFSIZ*2
int main(int argc, char **argv)
{
	size_t n;
	char in[INBUFSIZ];
	char buf[ENCBUFSIZ];
	encode bxe;
	enclen bxelen;

	memset(in, 0, INBUFSIZ);

	if (argc < 2 || argc > 3) {
		fprintf(stderr, "USAGE: bxe <file BASE [ALPHABET]\n");
		return 1;
	}

	if (strcmp(argv[1], "16") == 0) {
		if (argc == 3) bxa(16, b16a, argv[2]);
		bxe = b16e; bxelen = b16elen;

	} else if (strcmp(argv[1], "32") == 0) {
		if (argc == 3) bxa(32, b32a, argv[2]);
		bxe = b32e; bxelen = b32elen;

	} else if (strcmp(argv[1], "64") == 0) {
		if (argc == 3) bxa(64, b64a, argv[2]);
		bxe = b64e; bxelen = b64elen;

	} else if (strcmp(argv[1], "85") == 0) {
		if (argc == 3) bxa(85, b85a, argv[2]);
		bxe = b85e; bxelen = b85elen;

	} else {
		fprintf(stderr, "bxe: unrecognized base '%s'\n", argv[1]);
		return 2;
	}

	while ((n = read(0, in, INBUFSIZ)) > 0) {
		bxe(buf, in, n);
		write(1, buf, bxelen(n));
	}
	return 0;
}
