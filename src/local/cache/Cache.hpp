/**
 * @section license
 *
 * Copyright (c) 2006-2013 David Osborn
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

#ifndef    page_local_cache_Cache_hpp
#   define page_local_cache_Cache_hpp

#	include <functional> // function
#	include <memory> // shared_ptr
#	include <string>
#	include <typeinfo> // type_info
#	include <unordered_map>

#	include "../util/class/Monostate.hpp"

namespace page { namespace cache
{
////////// detail::CacheTime ///////////////////////////////////////////////////

	namespace detail
	{
		/**
		 * A structure used by @c Cache to measure time.  It uses multiple
		 * units, CacheTime::time and CacheTime::frame, to make a better
		 * decision about when to drop a Datum from the Cache.
		 */
		struct CacheTime
		{
			CacheTime() {}
			CacheTime(float time, unsigned frame) :
				time(time), frame(frame) {}

			/**
			 * The amount of time passed in seconds.
			 */
			float time = 0;

			/**
			 * The number of frames passed.
			 */
			unsigned frame = 0;
		};

		CacheTime operator -(const CacheTime &a, const CacheTime &b)
		{
			return CacheTime(
				a.time  - b.time,
				a.frame - b.frame);
		}

		bool operator <(const CacheTime &a, const CacheTime &b)
		{
			return
				a.time  < b.time &&
				a.frame < b.frame;
		}

		bool operator >(const CacheTime &a, const CacheTime &b)
		{
			return b < a;
		}
	}

////////// Cache ///////////////////////////////////////////////////////////////

	/**
	 * Temporary storage for objects that take up resources and can be re-
	 * created on-demand.
	 *
	 * @note It is not usually necessary to interact with the cache directly.
	 *       Check out @c Proxy and its derivatives, which are provided to
	 *       simplify the process of caching resources.
	 */
	class Cache :
		public util::Monostate<Cache>
	{
		/*--------------------------+
		| constructors & destructor |
		+--------------------------*/

		public:
		explicit Cache(const detail::CacheTime &lifetime = {4, 8});

		/*----------+
		| observers |
		+----------*/

		public:
		/**
		 * Fetches a piece of data from the cache.
		 */
		std::shared_ptr<const void> Fetch(
			const std::string &signature,
			const std::string &name,
			const std::type_info &) const;

		/**
		 * Fetches an object from the cache.
		 */
		template <typename T>
			std::shared_ptr<const T> Fetch(
				const std::string &signature,
				const std::string &name) const;

		/*----------+
		| modifiers |
		+----------*/

		public:
		/**
		 * Stores a piece of data in the cache.
		 */
		void Store(
			const std::string &signature,
			const std::string &name,
			const std::shared_ptr<const void> &,
			const std::type_info &,
			const std::function<void ()> &repair = nullptr);

		/**
		 * Stores an object in the cache.
		 */
		template <typename T>
			void Store(
				const std::string &signature,
				const std::string &name,
				const std::shared_ptr<const T> &data,
				const std::function<void ()> &repair = nullptr);

		/**
		 * Invalidates the matching datum.
		 */
		void Invalidate(
			const std::string &signature,
			const std::string &name);

		/**
		 * Purges all data from the cache.
		 */
		void Purge();

		/**
		 * Purges the matching datum from the cache.
		 */
		void Purge(
			const std::string &signature,
			const std::string &name);

		/**
		 * Purges the matching resource from the cache.
		 */
		void PurgeResource(
			const std::string &path);

		/*-------+
		| update |
		+-------*/

		public:
		/**
		 * Updates the cache, purging expired data.
		 *
		 * @note This function should be called by the main loop, exactly once
		 *       every frame.
		 */
		void Update(float deltaTime);

		/*-------------+
		| data members |
		+-------------*/

		private:
		/**
		 * A cached object.
		 */
		struct Datum
		{
			Datum(
				const std::string &name,
				const std::shared_ptr<const void> &data,
				const std::type_info &type,
				const std::function<void ()> &repair,
				const detail::CacheTime &atime = {}) :
					name(name),
					data(data),
					type(&type),
					repair(repair),
					atime(atime) {}

			/**
			 * The user-friendly name of the object.
			 */
			std::string name;

			/**
			 * The actual data.
			 */
			std::shared_ptr<const void> data;

			/**
			 * The type of the object.
			 */
			const std::type_info *type;

			/**
			 * A callback used to repair an @c invalid datum.
			 */
			std::function<void ()> repair;

			/**
			 * The time when the datum was last accessed.
			 */
			mutable detail::CacheTime atime;

			/**
			 * @c true if the datum has been invalidated.
			 */
			mutable bool invalid = false;
		};

		/**
		 * A pool containing all of the cached data, where each datum is
		 * accessible through its signature.
		 */
		std::unordered_map<std::string, Datum> pool;

		/**
		 * The length of time that data remains in the cache before it gets
		 * purged.
		 */
		const detail::CacheTime lifetime;

		/**
		 * A running count of how long the cache has been operating.
		 */
		detail::CacheTime time;
	};
}}

#	include "Cache.tpp"
#endif
