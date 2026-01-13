// #define MLK_CONFIG_API_NO_SUPERCOP

#include "__shared_config.h"
#include <cheri-builtins.h>
#include <debug.h>
#include <randombytes.h>
#include <stdlib.h>
#include <switcher.h>
#include <unwind.h>

// We have our own random source from the entropy compartment
#define MLK_CONFIG_CUSTOM_RANDOMBYTES
#define mlk_randombytes randombytes

// We provide a custom zeroing function: explicit_bzero
#define MLK_CONFIG_CUSTOM_ZEROIZE
#define mlk_zeroize explicit_bzero

// Public API functions should be qualified as libcalls.
#define MLK_CONFIG_EXTERNAL_API_QUALIFIER __cheriot_libcall

// Built the library in mlkem768 mode
// TODO: Parameterise this and expose the different variants.
#define MLK_CONFIG_NAMESPACE_PREFIX mlkem768
#define MLK_CONFIG_PARAMETER_SET 768

// Serial FIPS-202 means one copy of the SHA3 state instead of four, which is a
// big reduction in stack usage.
#define MLK_CONFIG_SERIAL_FIPS202_ONLY

// Context parameter is required to functions and is the allocator capability
// used to allocate the state.
#define MLK_CONFIG_CONTEXT_PARAMETER
#define MLK_CONFIG_CONTEXT_PARAMETER_TYPE AllocatorCapability

// We are providing custom allocation and deallocation functions that allocate
// against the heap capability that the caller provided.
#define MLK_CONFIG_CUSTOM_ALLOC_FREE

#define MLK_CUSTOM_ALLOC(value, type, count, context)                          \
	CHERIOT_PQC_CUSTOM_ALLOC(value, type, count, context)
#define MLK_CUSTOM_FREE(value, type, count, context)                           \
	CHERIOT_PQC_CUSTOM_FREE(value, type, count, context)
