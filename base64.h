#ifndef BASE64_H
#define BASE64_H

static inline size_t
b64elen(size_t dlen) {
	return 8 * (dlen / 5) + (dlen % 5);
}

static inline size_t
b64dlen(size_t elen) {
	return (elen / 8) * 5 + (elen % 8);
}

void b64a(const char *alpha);
int b64d(char *dst, const char *src, size_t len);
int b64e(char *dst, const char *src, size_t len);

#endif
