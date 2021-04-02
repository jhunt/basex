#include <stdio.h>
#include <stdlib.h>

#include "base16.h"
#include "base32.h"
#include "base64.h"
#include "base85.h"

int main(int argc, char **argv)
{
	int i, rc;
	long n;
	char *end;

	rc = 0;
	for (i = 1; i < argc; i++) {
		end = NULL;
		n = strtol(argv[i], &end, 10);
		if (end && *end) {
			fprintf(stderr, "bxl: '%s' does not look like a number to me...\n", argv[i]);
			rc = 1;
			break;
		}

		printf("%li bytes of data takes:\n", n);
		printf("  base-16: %li bytes\n", b16elen(n));
		printf("  base-32: %li bytes\n", b32elen(n));
		printf("  base-64: %li bytes\n", b64elen(n));
		printf("  base-85: %li bytes\n", b85elen(n));
		printf("\n");
		printf("%li bytes of encoded data represents:\n", n);
		printf("  %li bytes of base-16 encoded data\n", b16dlen(n));
		printf("  %li bytes of base-32 encoded data\n", b32dlen(n));
		printf("  %li bytes of base-64 encoded data\n", b64dlen(n));
		printf("  %li bytes of base-85 encoded data\n", b85dlen(n));
		printf("\n");
	}
}
