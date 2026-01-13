#ifdef CHERIOT_PQC_LIBRARY

// TODO: Expose these as options.
// Uncomment to log the stack high-water mark on free.
// #define CHERIOT_PQC_LOG_STACK_USAGE
// Uncomment to log heap usage.
// #define CHERIOT_PQC_DEBUG_ALLOCATION

#	ifdef CHERIOT_PQC_DEBUG_ALLOCATION
/**
 * Helper macro for debug logs on allocation.
 */
#		define CHERIOT_DEBUG_PQC_ALLOCATION_LOG(...)                          \
			CHERIOT_DEBUG_LOG(CHERIOT_PQC_LIBRARY, __VA_ARGS__)
#	else
#		define CHERIOT_DEBUG_PQC_ALLOCATION_LOG(...)
#	endif

// TODO: The timeout should probably be passed in as part of the context, but
// for now just fail instantly if there's a failure.
// The nested macros exist to support the timeout: it must be declared as a
// unique name.
#	define CHERIOT_PQC_CUSTOM_ALLOC(value, type, count, context)              \
		CHERIOT_PQC_CUSTOM_ALLOC2(value, type, count, context, __COUNTER__)
#	define CHERIOT_PQC_CUSTOM_ALLOC2(value, type, count, context, counter)    \
		CHERIOT_PQC_CUSTOM_ALLOC3(value, type, count, context, counter)
#	define CHERIOT_PQC_CUSTOM_ALLOC3(value, type, count, context, counter)    \
		Timeout timeout##counter = {0, 0};                                     \
		type   *value            = heap_allocate_array(                        \
          &timeout##counter, context, sizeof(type), count, 0);    \
		value = cheri_is_valid(value) ? value : NULL;                          \
		CHERIOT_DEBUG_PQC_ALLOCATION_LOG(                                      \
		  #type " " #value ": Heap allocation of {} bytes: {}",                \
		  (int)(sizeof(type) * count),                                         \
		  value);

#	ifdef CHERIOT_CHERIOT_PQC_LOG_STACK_USAGE
#		define PQC_CUSTOM_FREE(value, type, count, context)                    \
			ptraddr_t stackTop    = cheri_top_get(__builtin_cheri_stack_get()); \
			ptraddr_t stackLowest = stack_lowest_used_address();                \
			CHERIOT_DEBUG_LOG(CHERIOT_PQC_LIBRARY,                              \
			                  "Stack usage on free: {} bytes",                  \
			                  (int)(stackTop - stackLowest));                   \
			CHERIOT_DEBUG_PQC_ALLOCATION_LOG(                                   \
			  #type " " #value ": Freeing heap allocation of {} bytes: {}",     \
			  (int)(sizeof(type) * count),                                      \
			  value);                                                           \
			heap_free(context, (void *)value);
#	else
#		define CHERIOT_PQC_CUSTOM_FREE(value, type, count, context)           \
			heap_free(context, (void *)value)
#	endif

#endif
