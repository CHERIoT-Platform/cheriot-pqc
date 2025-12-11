//#define MLK_CONFIG_API_NO_SUPERCOP
#define MLK_CONFIG_CUSTOM_RANDOMBYTES
#define MLK_CONFIG_CUSTOM_ZEROIZE
#define MLK_CONFIG_EXTERNAL_API_QUALIFIER __cheriot_libcall
#define MLK_CONFIG_NAMESPACE_PREFIX mlkem768
#define MLK_CONFIG_PARAMETER_SET 768
#define mlk_randombytes randombytes
#define mlk_zeroize explicit_bzero

#include <randombytes.h>
