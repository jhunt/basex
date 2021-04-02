#ifndef BASE64_H
#define BASE64_H

#define BASE64_ALPHA     "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/"
#define BASE64_URL_ALPHA "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789-_"

static inline size_t
b64elen(size_t dlen) {
	return (8*dlen) / 6 + (8*dlen % 6 == 0 ? 0 : 1);
}

static inline size_t
b64dlen(size_t elen) {
	return (6*elen) / 8;
}

void b64a(const char *alpha);
int b64d(char *dst, const char *src, size_t len);
int b64e(char *dst, const char *src, size_t len);

#endif
