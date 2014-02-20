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

#ifndef    page_local_cache_Cache_hpp
#   define page_local_cache_Cache_hpp

#	include <functional> // function
#	include <memory> // shared_ptr
#	include <string>
#	include <typeinfo> // type_info
#	include <unordered_map>

#	include "../util/class/Monostate.hpp"
#	include "Signature.hpp"

namespace page { namespace cache
{
////////// detail::CacheTime ///////////////////////////////////////////////////

	namespace detail
	{
		/**
		 * A data structure used by Cache to measure time.  It uses multiple
		 * units to make a better decision about when to drop a datum from the
		 * cache.
		 */
		struct CacheTime
		{
			CacheTime() noexcept = default;
			CacheTime(float time, unsigned frame) noexcept :
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

		CacheTime operator -(const CacheTime &a, const CacheTime &b) noexcept
		{
			return CacheTime(
				a.time  - b.time,
				a.frame - b.frame);
		}

		bool operator ==(const CacheTime &a, const CacheTime &b) noexcept
		{
			return
				a.time  == b.time &&
				a.frame == b.frame;
		}

		bool operator !=(const CacheTime &a, const CacheTime &b) noexcept
		{
			return
				a.time  != b.time &&
				a.frame != b.frame;
		}

		bool operator <=(const CacheTime &a, const CacheTime &b) noexcept
		{
			return
				a.time  <= b.time &&
				a.frame <= b.frame;
		}

		bool operator >=(const CacheTime &a, const CacheTime &b) noexcept
		{
			return
				a.time  >= b.time &&
				a.frame >= b.frame;
		}

		bool operator <(const CacheTime &a, const CacheTime &b) noexcept
		{
			return
				a.time  < b.time &&
				a.frame < b.frame;
		}

		bool operator >(const CacheTime &a, const CacheTime &b) noexcept
		{
			return
				a.time  > b.time &&
				a.frame > b.frame;
		}
	}

////////// Cache ///////////////////////////////////////////////////////////////

	/**
	 * Temporary storage for objects that take up resources and can be re-
	 * created on-demand.
	 *
	 * @note It is not usually necessary to interact with the cache directly.
	 *       Check out Proxy and its related classes, which have been provided
	 *       to simplify the process of caching resources.
	 *
	 * @fixme This class should track the usage patterns of its data and adjust
	 *        their lifetimes for optimal balancing between cache coherence and
	 *        memory management.
	 */
	class Cache : public util::Monostate<Cache>
	{
		/*-------------+
		| constructors |
		+-------------*/

		public:
		explicit Cache(const detail::CacheTime &lifetime = {4, 8});

		/*-----------------+
		| cache operations |
		+-----------------*/

		/**
		 * Fetches data from the cache.
		 *
		 * @param[in] signature A signature that uniquely identifies the datum.
		 * @param[in] type The type of the datum.
		 */
		std::shared_ptr<const void> Fetch(
			Signature      const& signature,
			std::type_info const& type) const;

		/**
		 * Fetches an object from the cache.
		 *
		 * @param[in] signature A signature that uniquely identifies the datum.
		 */
		template <typename T>
			std::shared_ptr<const T> Fetch(const Signature &signature) const;

		/**
		 * Stores data in the cache.
		 *
		 * @param[in] signature A signature that uniquely identifies the datum.
		 * @param[in] data The data to store.
		 * @param[in] type The type of the datum.
		 * @param[in] repair A function that can be used to restore the datum
		 *            after it has been invalidated.
		 */
		void Store(
			Signature                   const& signature,
			std::shared_ptr<const void> const& data,
			std::type_info              const& type,
			std::function<void ()>      const& repair = nullptr);

		/**
		 * Stores an object in the cache.
		 *
		 * @param[in] signature A signature that uniquely identifies the datum.
		 * @param[in] data The data to store.
		 * @param[in] repair A function that can be used to restore the datum
		 *            after it has been invalidated.
		 */
		template <typename T>
			void Store(
				Signature                const& signature,
				std::shared_ptr<const T> const& data,
				std::function<void ()>   const& repair = nullptr);

		/**
		 * Touches the matching datum, updating its access time.
		 *
		 * @param[in] signature A signature that uniquely identifies the datum.
		 */
		void Touch(const Signature &signature);

		/**
		 * Invalidates the matching datum.
		 *
		 * @param[in] signature A signature that uniquely identifies the datum.
		 */
		void Invalidate(const Signature &signature);

		/**
		 * Purges all data from the cache.
		 */
		void Purge();

		/**
		 * Purges the matching datum from the cache.
		 *
		 * @param[in] signature A signature that uniquely identifies the datum.
		 */
		void Purge(const Signature &signature);

		/**
		 * Purges all matching resource data from the cache.
		 *
		 * @param[in] path The path of the resource.
		 */
		void PurgeResource(const std::string &path);

		/*----------------+
		| other functions |
		+----------------*/

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
				std::shared_ptr<const void> const& data,
				std::type_info              const& type,
				std::function<void ()>      const& repair,
				detail::CacheTime           const& atime = {}) :
					data(data),
					type(&type),
					repair(repair),
					atime(atime) {}

			/**
			 * The actual data.
			 */
			std::shared_ptr<const void> data;

			/**
			 * The type of the object.
			 */
			const std::type_info *type;

			/**
			 * A callback used to repair an invalid datum.
			 */
			std::function<void ()> repair;

			/**
			 * The time when the datum was last accessed.
			 */
			mutable detail::CacheTime atime;

			/**
			 * true if the datum has been invalidated.
			 */
			mutable bool invalid = false;
		};

		/**
		 * A pool containing all of the cached data, where each datum is keyed
		 * by its signature.
		 */
		std::unordered_map<Signature, Datum> pool;

		/**
		 * The length of time that data will remain in the cache before being
		 * purged.
		 */
		const detail::CacheTime lifetime;

		/**
		 * A running count of how long the cache has been operational.
		 */
		detail::CacheTime time;
	};
}}

#	include "Cache.tpp"
#endif
