/**
 * @section license
 *
 * Copyright (c) 2006-2012 David Osborn
 *
 * Permission is granted to use and redistribute this software in source and
 * binary form, with or without modification, subject to the following
 * conditions:
 *
 * 1. Redistributions in source form must retain the above copyright notice,
 *    this list of conditions, and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions, and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution, and in the same
 *    place and form as other copyright, license, and disclaimer information.
 *
 * As a special exception, distributions of derivative works in binary form may
 * include an acknowledgement in place of the above copyright notice, this list
 * of conditions, and the following disclaimer in the documentation and/or other
 * materials provided with the distribution, and in the same place and form as
 * other acknowledgements, similar in substance to the following:
 *
 *    Portions of this software are based on the work of David Osborn.
 *
 * This software is provided "as is", without any express or implied warranty.
 * In no event will the authors be liable for any damages arising out of the use
 * of this software.
 */

#ifndef    page_local_util_functional_member_hpp
#   define page_local_util_functional_member_hpp

namespace page
{
	namespace util
	{
		/**
		 * @defgroup functional-member
		 *
		 * Function objects for interacting with members.
		 *
		 * @{
		 */
		/**
		 * A function object for extracting a member of an aggregate.
		 *
		 * @note @c std::mem_fn is probably a better choice if you need to
		 *       access a member function rather than a data member.
		 *       @c std::mem_fn takes a pointer-to-member-function (ie: &T::F)
		 *       and returns a function object that calls the member.
		 */
		template <typename T, typename M>
			struct member_of
		{
			explicit member_of(M T::*m) : m(m) {}

			M &operator ()(T &t) const
			{
				return t.*m;
			}
			const M &operator ()(const T &t) const
			{
				return t.*m;
			}

			private:
			M T::*m;
		};

		/**
		 * A convenience function for creating an instance of @c member_of.
		 */
		template <typename T, typename M>
			member_of<T, M> make_member_of(M T::*m)
		{
			return member_of<T, M>(m);
		}
		///@}
	}
}

#endif
