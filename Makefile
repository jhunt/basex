default: test

TESTS := base16 base32 base64 base85
ALL_THAT_BASE := base16.o base32.o base64.o base85.o

test: CFLAGS += -DO_TESTS
test: $(TESTS)
	prove -v $(patsubst %, ./%, $+)

utils: alphagen bxd bxe bxl

alphagen.o: util/alphagen.c
	$(CC) -I. -c -o $@ $+

bxd: $(ALL_THAT_BASE)
bxd.o: util/bxd.c
	$(CC) -I. -c -o $@ $+

bxe: $(ALL_THAT_BASE)
bxe.o: util/bxe.c
	$(CC) -I. -c -o $@ $+

bxl: $(ALL_THAT_BASE)
bxl.o: util/bxl.c
	$(CC) -I. -c -o $@ $+

clean:
	rm -f *.o
	rm -f $(TESTS)
