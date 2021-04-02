#ifndef BASE85_H
#define BASE85_H

static inline size_t
b85elen(size_t dlen) {
	return (5*dlen) / 4 + (5*dlen % 4 == 0 ? 0 : 1);
}

static inline size_t
b85dlen(size_t elen) {
	return (4*elen) / 5;
}

void b85a(const char *alpha);
int b85d(char *dst, const char *src, size_t len);
int b85e(char *dst, const char *src, size_t len);

#endif
