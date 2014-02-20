/**
 * @copyright
 *
 * Copyright (c) 2006-2014 David Osborn
 *
 * Permission is granted to use and redistribute this software in source and
 * binary form, with or without modification, subject to the following
 * conditions:
 *
 * 1. Redistributions in source form must retain the above copyright notice,
 *    this list of conditions, and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions, and the following disclaimer in the same place
 *    and form as other copyright, license, and disclaimer information.
 *
 * 3. Redistributions in binary form must also include an acknowledgement in the
 *    same place and form as other acknowledgements (such as the credits),
 *    similar in substance to the following:
 *
 *       Portions of this software are based on the work of David Osborn.
 *
 * This software is provided "as is", without any express or implied warranty.
 * In no event will the authors be liable for any damages arising out of the use
 * of this software.
 */

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
