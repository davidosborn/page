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

#include <cassert>
#include <iostream> // cout

#include <boost/optional.hpp>

#include "../cfg/vars.hpp"
#include "../err/report.hpp" // ReportWarning, std::exception
#include "../log/Indenter.hpp"
#include "../log/Stats.hpp"
#include "Cache.hpp"

namespace page
{
	namespace cache
	{
		/*--------------------------+
		| constructors & destructor |
		+--------------------------*/

		Cache::Cache(const detail::CacheTime &lifetime) :
			lifetime(lifetime) {}

		/*----------+
		| observers |
		+----------*/

		std::shared_ptr<const void>
			Cache::Fetch(
				const std::string &signature,
				const std::string &name,
				const std::type_info &type) const
		{
			GLOBAL(log::Stats).IncCacheTries();

			// look for a matching datum
			auto iter(pool.find(signature));
			if (iter != pool.end())
			{
				const auto &datum(iter->second);
				assert(*datum.type == type);

				// if the datum is invalid, we need to repair it
				if (datum.invalid)
				{
					assert(datum.repair);

					// write to the log
					boost::optional<log::Indenter> indenter;
					if (*CVAR(logCache) && *CVAR(logCacheUpdate))
					{
						std::cout << "updating cached " << name << std::endl;
						indenter = boost::in_place();
					}

					// attempt to repair the invalid datum
					try
					{
						datum.repair();
						datum.invalid = false;
					}
					catch (const std::exception &e)
					{
						err::ReportWarning(e);
						const_cast<decltype(pool) &>(pool).erase(iter);
						return nullptr;
					}
				}

				// update the access time and return the data
				datum.atime = time;
				return datum.data;
			}

			// no matching datum was found
			GLOBAL(log::Stats).IncCacheMisses();
			if (*CVAR(logCache) && *CVAR(logVerbose))
				std::cout << "cache missing " << name << std::endl;
			return nullptr;
		}

		/*----------+
		| modifiers |
		+----------*/

		void Cache::Store(
			const std::string &signature,
			const std::string &name,
			const std::shared_ptr<const void> &data,
			const std::type_info &type,
			const std::function<void ()> &repair)
		{
			// write to the log
			boost::optional<log::Indenter> indenter;
			if (*CVAR(logCache))
			{
				std::cout << "caching " << name << std::endl;
				indenter = boost::in_place();
			}

			pool.insert(std::make_pair(signature, Datum(name, data, type, repair, time)));
		}

		void Cache::Invalidate(
			const std::string &signature,
			const std::string &name)
		{
			// write to the log
			boost::optional<log::Indenter> indenter;
			if (*CVAR(logCache) && *CVAR(logCacheUpdate) && *CVAR(logVerbose))
			{
				std::cout << "invalidating cached " << name << std::endl;
				indenter = boost::in_place();
			}

			auto iter(pool.find(signature));
			if (iter != pool.end())
			{
				auto &datum(iter->second);
				if (datum.repair) datum.invalid = true;
				else pool.erase(iter);
			}
		}

		void Cache::Purge()
		{
			// write to the log
			boost::optional<log::Indenter> indenter;
			if (*CVAR(logCache))
			{
				std::cout << "purging cache" << std::endl;
				indenter = boost::in_place();
			}

			pool.clear();
			time = {};
		}

		void Cache::Purge(
			const std::string &signature,
			const std::string &name)
		{
			// write to the log
			boost::optional<log::Indenter> indenter;
			if (*CVAR(logCache))
			{
				std::cout << "purging cached " << name << std::endl;
				indenter = boost::in_place();
			}

			pool.erase(signature);
		}

		void Cache::PurgeResource(const std::string &path)
		{
			// FIXME: implement
		}

		/*-------+
		| update |
		+-------*/

		void Cache::Update(float deltaTime)
		{
			// update current time
			time.time += deltaTime;
			++time.frame;

			// drop expired datums
			for (auto iter(pool.begin()); iter != pool.end();)
			{
				Datum &datum(iter->second);
				if (time - datum.atime > lifetime && datum.data.unique())
				{
					// write to the log
					boost::optional<log::Indenter> indenter;
					if (*CVAR(logCache) && *CVAR(logVerbose))
					{
						std::cout << "cached " << datum.name << " timed out" << std::endl;
						indenter = boost::in_place();
					}

					pool.erase(iter++);
				}
				else ++iter;
			}
		}
	}
}
