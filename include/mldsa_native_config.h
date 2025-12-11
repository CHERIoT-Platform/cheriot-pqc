#include <randombytes.h>

#define MLD_CONFIG_NO_SUPERCOP
#define MLD_CONFIG_CUSTOM_RANDOMBYTES
#define MLD_CONFIG_CUSTOM_ZEROIZE
#define MLD_CONFIG_EXTERNAL_API_QUALIFIER __cheriot_libcall
#define MLD_CONFIG_NAMESPACE_PREFIX mldsa44
#define MLD_CONFIG_PARAMETER_SET 44
#define mld_zeroize_native explicit_bzero
#define mld_randombytes randombytes
