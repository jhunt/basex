default: test

TESTS := base16 base32 base64

test: CFLAGS += -DO_TESTS
test: $(TESTS)
	prove -v $(patsubst %, ./%, $+)

alphagen.o: util/alphagen.c
	$(CC) -c -o $@ $+

clean:
	rm -f *.o
	rm -f $(TESTS)
