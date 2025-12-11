Experimental Post-Quantum Crypto Support for CHERIoT
====================================================

This repository contains experimental support for post-quantum encryption algorithms adapted to CHERIoT.
The implementations are unmodified from the Linux Foundation's pq-code-package/mldsa-native and pq-code-package/mlkem-native packages.
They have been wrapped to build as CHERIoT libraries.

This provides implementations of ML-KEM (FIPS 203) and ML-DSA (FIPS 204).
Both depend internally on SHA3 (FIPS 202).

Experimental APIs
-----------------

Currently, the two libraries expose the underlying APIs.
These are not ideally adapted for CHERIoT and are subject to change.

Currently, both the ML-KEM and ML-DSA include a SHA3 implementation but neither exposes it.
Hopefully a future version will separate this out and expose SHA3 functionality directly.

Future changes will also allow using heap instead of stack memory.

The current version implements only a shared library for the PQC algorithms.
This means key management and so on are entirely the responsibility of the caller.
A future version will wrap keys in sealed capabilities.

Note on stack usage
-------------------

Both implementations currently use a lot more stack space than most embedded targets provide.
A typical CHERIoT thread stack is under 2 KiB.
The ML-KEM implementation uses around 18 KiB (slightly more for decryption than encryption).
The ML-DSA implementation uses around 60 KiB to sign and around 48 KiB to 
