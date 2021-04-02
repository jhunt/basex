default: test

TESTS := base16 base32 base64

test: CFLAGS += -DO_TESTS
test: $(TESTS)
	prove -v $(patsubst %, ./%, $+)

alphagen.o: util/alphagen.c
	$(CC) -I. -c -o $@ $+

bxd: base16.o base32.o base64.o
bxd.o: util/bxd.c
	$(CC) -I. -c -o $@ $+

bxe: base16.o base32.o base64.o
bxe.o: util/bxe.c
	$(CC) -I. -c -o $@ $+

clean:
	rm -f *.o
	rm -f $(TESTS)
