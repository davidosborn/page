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

// C++
#include <cassert>
#include <functional> // function
#include <iostream> // cout
#include <memory> // shared_ptr
#include <string>
#include <typeinfo> // type_info
#include <unordered_map>

// Boost
#include <boost/optional.hpp>

// local
#include "cfg/vars.hpp"
#include "err/report.hpp" // ReportWarning, std::exception
#include "log/Indenter.hpp"
#include "log/stats.hpp" // GetStats, Stats::Inc{Misses,Tries}

namespace page
{
	namespace cache
	{
		namespace
		{
			// counter
			struct Count
			{
				float time;
				unsigned frame;
			};
			const Count duration = {4, 8};
			Count count;

			// counter operators
			inline Count operator -(const Count &a, const Count &b)
			{
				Count r =
				{
					a.time  - b.time,
					a.frame - b.frame
				};
				return r;
			}
			inline bool operator <(const Count &a, const Count &b)
			{
				return
					a.time  < b.time &&
					a.frame < b.frame;
			}

			// storage
			struct Datum
			{
				std::string name;
				std::shared_ptr<const void> data;
				const std::type_info *type;
				std::function<void ()> repair;
				mutable Count atime;
				mutable bool dirty;
			};
			typedef std::unordered_map<std::string, Datum> Cache;
			inline Cache &GetCache()
			{
				static Cache cache;
				return cache;
			}
		}

		// fetch
		std::shared_ptr<const void> FetchRaw(const std::string &sig, const std::string &name, const std::type_info &type)
		{
			log::GetStats().IncCacheTries();
			Cache::iterator iter(GetCache().find(sig));
			if (iter != GetCache().end())
			{
				const Datum &datum(iter->second);
				assert(*datum.type == type);
				if (datum.dirty)
				{
					assert(datum.repair);
					boost::optional<log::Indenter> indenter;
					if (CVAR(logCache) && CVAR(logCacheUpdate))
					{
						std::cout << "updating cached " << name << std::endl;
						indenter = boost::in_place();
					}
					try
					{
						datum.repair();
					}
					catch (const std::exception &e)
					{
						err::ReportWarning(e);
						GetCache().erase(iter);
						return std::shared_ptr<const void>();
					}
					datum.dirty = false;
				}
				datum.atime = count;
				return datum.data;
			}
			log::GetStats().IncCacheMisses();
			if (CVAR(logCache) && CVAR(logVerbose))
				std::cout << "cache missing " << name << std::endl;
			return std::shared_ptr<const void>();
		}

		// store
		void StoreRaw(const std::string &sig, const std::string &name, const std::shared_ptr<const void> &data, const std::type_info &type, const std::function<void ()> &repair)
		{
			boost::optional<log::Indenter> indenter;
			if (CVAR(logCache))
			{
				std::cout << "caching " << name << std::endl;
				indenter = boost::in_place();
			}
			Datum datum = {name, data, &type, repair, count};
			GetCache().insert(std::make_pair(sig, datum));
		}

		// invalidation
		void Invalidate(const std::string &sig, const std::string &name)
		{
			boost::optional<log::Indenter> indenter;
			if (CVAR(logCache) && CVAR(logCacheUpdate) && CVAR(logVerbose))
			{
				std::cout << "invalidating cached " << name << std::endl;
				indenter = boost::in_place();
			}
			Cache::iterator iter(GetCache().find(sig));
			if (iter != GetCache().end())
			{
				Datum &datum(iter->second);
				if (datum.repair) datum.dirty = true;
				else GetCache().erase(iter);
			}
		}

		// purge
		void Purge()
		{
			boost::optional<log::Indenter> indenter;
			if (CVAR(logCache))
			{
				std::cout << "purging cache" << std::endl;
				indenter = boost::in_place();
			}
			GetCache().clear();
			count = Count();
		}
		void Purge(const std::string &sig, const std::string &name)
		{
			boost::optional<log::Indenter> indenter;
			if (CVAR(logCache))
			{
				std::cout << "purging cached " << name << std::endl;
				indenter = boost::in_place();
			}
			GetCache().erase(sig);
		}
		void PurgeResource(const std::string &path)
		{
			// FIXME: implement
		}

		// update
		void Update(float deltaTime)
		{
			// update counter
			count.time += deltaTime;
			++count.frame;
			// drop expired datums
			for (Cache::iterator iter(GetCache().begin()); iter != GetCache().end();)
			{
				Datum &datum(iter->second);
				if (duration < count - datum.atime && datum.data.unique())
				{
					boost::optional<log::Indenter> indenter;
					if (CVAR(logCache) && CVAR(logVerbose))
					{
						std::cout << "cached " << datum.name << " timed out" << std::endl;
						indenter = boost::in_place();
					}
					GetCache().erase(iter++);
				}
				else ++iter;
			}
		}
	}
}
