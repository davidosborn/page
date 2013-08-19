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
		 * A data structure used by @c Cache to measure time.  It uses multiple
		 * units, @a time and @a frame, to make a better decision about when to
		 * drop a datum from the cache.
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
	 *       Check out @c Proxy and its derivatives, which are provided to
	 *       simplify the process of caching resources.
	 *
	 * @fixme This class should track the usage patterns of its data and adjust
	 *        their lifetimes for optimal balancing between cache coherence and
	 *        memory management.
	 */
	class Cache :
		public util::Monostate<Cache>
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
		std::unordered_map<Signature, Datum> pool;

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
