#include "__shared_config.h"
#include <cheri-builtins.h>
#include <debug.h>
#include <randombytes.h>
#include <stdlib.h>
#include <switcher.h>

// Don't expose the un-namespaced `CRYPTO_*` macros
#define MLD_CONFIG_NO_SUPERCOP

// Get random data by calling the entropy compartment.
#define MLD_CONFIG_CUSTOM_RANDOMBYTES
#define mld_randombytes randombytes

// Use explicit_bzero for zeroing
#define MLD_CONFIG_CUSTOM_ZEROIZE
#define mld_zeroize explicit_bzero

// Public APIs are `__cheriot_libcall`-qualified
#define MLD_CONFIG_EXTERNAL_API_QUALIFIER __cheriot_libcall

// Use ML-DSA-44 configuration
// TODO: Support alternatives / more than one variant.
#define MLD_CONFIG_NAMESPACE_PREFIX mldsa44
#define MLD_CONFIG_PARAMETER_SET 44

// Serial FIPS-202 means one copy of the SHA3 state instead of four, which is a
// big reduction in stack usage.
// TODO: This is a small speedup with ML-KEM, but it's a slowdown with ML-DSA.
// It reduces peak stack usage from about 6 KiB to about 3 KiB though, so worth
// it for now.
#define MLD_CONFIG_SERIAL_FIPS202_ONLY

// Context parameter is required to functions and is the allocator capability
// used to allocate the state.
#define MLD_CONFIG_CONTEXT_PARAMETER
#define MLD_CONFIG_CONTEXT_PARAMETER_TYPE AllocatorCapability

// We are providing custom allocation and deallocation functions that allocate
// against the heap capability that the caller provided.
#define MLD_CONFIG_CUSTOM_ALLOC_FREE

#define MLD_CUSTOM_ALLOC(value, type, count, context)                          \
	CHERIOT_PQC_CUSTOM_ALLOC(value, type, count, context)
#define MLD_CUSTOM_FREE(value, type, count, context)                           \
	CHERIOT_PQC_CUSTOM_FREE(value, type, count, context)
