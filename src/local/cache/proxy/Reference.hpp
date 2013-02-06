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

// proxy adapter for non-cached objects
// uses local storage to avoid collisions and redundant hash lookups

#ifndef    page_local_cache_proxy_Reference_hpp
#   define page_local_cache_proxy_Reference_hpp

#	include "../Proxy.hpp"

namespace page
{
	namespace cache
	{
		template <typename T> struct Reference : Proxy<T>
		{
			// HACK: GCC doesn't support using typename (bug #14258)
			typedef typename Proxy<T>::Instance Instance;

			// construct
			explicit Reference(const T &);
			explicit Reference(const Instance & = Instance());

			// clone
			Reference<T> *Clone() const;

			// access
			Instance lock() const;

			// attributes
			std::string GetType() const;
			std::string GetSource() const;

			// dependency satisfaction
			operator bool() const;

			// modifiers
			void Invalidate() const;
			void Purge() const;

			private:
			// instantiation
			Instance Make() const;

			Instance instance;
			std::string source;
		};
	}
}

#	include "Reference.tpp"
#endif
