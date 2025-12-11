Simple PQC benchmark
====================

This example shows how to use the ML-KEM and ML-DSA APIs to:

 - Generate an ML-KEM key pair.
 - Generate a shared secret and its encrypted version with ML-KEM.
 - Decrypt the shared secret with ML-KEM.
 - Generate an ML-DSA key pair.
 - Sign a message with ML-DSA.
 - Verify a message signature with ML-DSA.

Each operation reports the stack size and number of cycles.

This example defaults to compiling for the CHERIoT Ibex simulator so that the reported timings reflect real hardware.
