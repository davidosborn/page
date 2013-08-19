#ifndef    page_local_util_gcc_init_priority_hpp
#   define page_local_util_gcc_init_priority_hpp

#	define MIN_INIT_PRIORITY 101
#	define MAX_INIT_PRIORITY 65535

	/**
	 * The value of the @c init_priority attribute for registering a product
	 * with a factory.
	 */
#	define REG_INIT_PRIORITY (MIN_INIT_PRIORITY + 1)

	/**
	 * The value of the @c init_priority attribute for initializing the logging
	 * system.
	 */
#	define LOG_INIT_PRIORITY (MIN_INIT_PRIORITY + 2)

	/**
	 * The value of the @c init_priority attribute for initializing the error-
	 * handling system.
	 */
#	define ERR_INIT_PRIORITY (MIN_INIT_PRIORITY + 3)

#endif
