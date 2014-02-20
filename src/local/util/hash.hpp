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

#ifndef    page_local_util_hash_hpp
#   define page_local_util_hash_hpp

#	include <cstddef> // size_t
#	include <functional> // binary_function

namespace page
{
	namespace util
	{
		// hash functions
		template <typename T> std::size_t make_hash(const T &);
		std::size_t combine_hash(std::size_t, std::size_t);
		template <typename InputIterator> std::size_t hash_range(InputIterator first, InputIterator last);

		// hash function object wrappers
		struct combine_hash_function : std::binary_function<std::size_t, std::size_t, std::size_t>
		{
			std::size_t operator ()(std::size_t, std::size_t) const;
		};

		// Paul Hsieh's fast hash function
		std::size_t SuperFastHash(const char *, unsigned n, std::size_t seed);
		std::size_t SuperFastHash(const char *, unsigned n);
	}
}

#	include "hash.tpp"
#endif
