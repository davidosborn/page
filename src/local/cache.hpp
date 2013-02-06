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

#ifndef    page_local_cache_hpp
#   define page_local_cache_hpp

#	include <functional> // function
#	include <memory> // shared_ptr
#	include <string>
#	include <typeinfo> // type_info

namespace page
{
	namespace cache
	{
		// fetch
		std::shared_ptr<const void> FetchRaw(
			const std::string &signature,
			const std::string &name,
			const std::type_info &);
		template <typename T>
			std::shared_ptr<const T> Fetch(
				const std::string &signature,
				const std::string &name);

		// store
		void StoreRaw(
			const std::string &signature,
			const std::string &name,
			const std::shared_ptr<const void> &,
			const std::type_info &,
			const std::function<void ()> &repair = 0);
		template <typename T>
			void Store(
				const std::string &signature,
				const std::string &name,
				const std::shared_ptr<const T> &data,
				const std::function<void ()> &repair = 0);

		// invalidation
		void Invalidate(
			const std::string &signature,
			const std::string &name);

		// purge
		void Purge();
		void Purge(const std::string &signature, const std::string &name);
		void PurgeResource(const std::string &path);

		// update
		void Update(float deltaTime);
	}
}

#	include "cache.tpp"
#endif
