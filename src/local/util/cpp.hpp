#ifndef    page_local_util_cpp_hpp
#   define page_local_util_cpp_hpp

	/**
	 * @defgroup STRINGIZE
	 * @{
	 */
#	define STRINGIZE2(x) #x

	/**
	 * Convert the macro argument into a string literal.
	 *
	 * @note This macro is also defined in config.h, to make it available to the
	 *       build system.
	 */
#	define STRINGIZE(x) STRINGIZE2(x)
	///@}

	/**
	 * When used in place of a comma inside a template parameter list, this
	 * allows a multi-parameter template to be passed to a macro.
	 */
#	define COMMA ,

#endif
