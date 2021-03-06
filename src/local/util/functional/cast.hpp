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

#ifndef    page_local_util_functional_cast_hpp
#   define page_local_util_functional_cast_hpp

namespace page
{
	namespace util
	{
		/**
		 * @defgroup functional-cast
		 *
		 * Function objects that wrap the type-cast operators.
		 *
		 * @{
		 */
#	define DEFINE_CAST_FUNCTION(TYPE) \
		template <typename T, typename U> \
			struct TYPE##_cast_function \
		{ \
			T operator ()(U x) const \
			{ \
				return TYPE##_cast<T>(x); \
			} \
		};

		DEFINE_CAST_FUNCTION(const)
		DEFINE_CAST_FUNCTION(dynamic)
		DEFINE_CAST_FUNCTION(reinterpret)
		DEFINE_CAST_FUNCTION(static)

#	undef DEFINE_CAST_FUNCTION
		///@}
	}
}

#endif
