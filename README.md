base{16,32,64}
==============

This repository contains standalone, unencumbered definitions of
the base-16, base-32, and base-64 binary-to-text encoding
algorithms, as outlined in [RFC 4648][1].

Usage
-----

To use this code, copy the appropriate `.h` and `.c` files into
your project, wherever it makes sense.  Each implementation is
completely separate from the others.

Hacking
-------

To run tests:

    $ make test

[1]: https://tools.ietf.org/html/rfc4648
