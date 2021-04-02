default: test

TESTS := base16 base32 base64

test: CFLAGS += -DO_TESTS
test: $(TESTS)
	prove -v $(patsubst %, ./%, $+)

clean:
	rm -f *.o
	rm -f $(TESTS)
