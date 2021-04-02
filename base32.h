#ifndef __NETIP_BASE32_H
#define __NETIP_BASE32_H

static inline size_t
b32elen(size_t dlen) {
	return 8 * (dlen / 5) + (dlen % 5);
}

static inline size_t
b32dlen(size_t elen) {
	return (elen / 8) * 5 + (elen % 8);
}

void b32a(const char *alpha);
int b32d(char *dst, const char *src, size_t len);
int b32e(char *dst, const char *src, size_t len);

#endif
