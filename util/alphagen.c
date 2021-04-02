#include <stdio.h>
#include <string.h>
#include <stddef.h>

#define TABSIZ 256

struct alias {
	const char *name;
	const char *alpha;
};
static struct alias aliases[] = {
	{"base16",     "0123456789abcdef"},
	{"base32",     "abcdefghijklmnopqrstuvwxyz234567"},
	{"base32-hex", "0123456789abcdefghijklmnopqrstuv"},
	{"base64",     "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/"},
	{"base64-url", "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789-_"},
	{NULL, NULL},
};

int main(int argc, char **argv) {
	const char *alpha;
	char lookup[TABSIZ];
	size_t i;

	if (argc != 2) {
		fprintf(stderr, "USAGE: alphagen ALPHABET-STRING\n");
		return 1;
	}

	alpha = argv[1];
	for (i = 0; aliases[i].name; i++) {
		if (strcmp(alpha, aliases[i].name) == 0) {
			alpha = aliases[i].alpha;
			break;
		}
	}

	memset(lookup, 0, TABSIZ);
	for (i = 0; i < strlen(alpha); i++) {
		lookup[alpha[i] & 0xff] = i;
	}

	printf("static char ALPHA[%ld] = \"%s\";\n", strlen(alpha), alpha);
	printf("static char LOOKUP[%d] = {\n\t", TABSIZ);
	for (i = 0; i < TABSIZ; i++) {
		if (i) printf(i % 16 == 0 ? "\n\t" : " ");

		if (lookup[i]) printf("%#04x,", lookup[i]);
		else           printf("   0,");
	}
	printf("\n};\n");
	return 0;
}
