#include <cassert>
#include <iostream> // cout

#include <boost/algorithm/hex.hpp>
#include <boost/optional.hpp>

#include "../cfg/vars.hpp"
#include "../err/report.hpp" // ReportWarning, std::exception
#include "../log/Indenter.hpp"
#include "../log/Stats.hpp"
#include "Cache.hpp"

namespace page { namespace cache
{
	/*-------------+
	| constructors |
	+-------------*/

	Cache::Cache(const detail::CacheTime &lifetime) :
		lifetime(lifetime) {}

	/*-----------------+
	| cache operations |
	+-----------------*/

	std::shared_ptr<const void>
		Cache::Fetch(
			const Signature &signature,
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
					std::cout << "updating cached object: " << signature << std::endl;
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
			std::cout << "cache missing object: " << signature << std::endl;
		return nullptr;
	}

	void Cache::Store(
		const Signature &signature,
		const std::shared_ptr<const void> &data,
		const std::type_info &type,
		const std::function<void ()> &repair)
	{
		// write to the log
		boost::optional<log::Indenter> indenter;
		if (*CVAR(logCache))
		{
			std::cout << "caching object: " << signature << std::endl;
			indenter = boost::in_place();
		}

		pool.insert({signature, Datum(data, type, repair, time)});
	}

	void Cache::Touch(const Signature &signature)
	{
		// write to the log
		boost::optional<log::Indenter> indenter;
		if (*CVAR(logCache) && *CVAR(logCacheUpdate) && *CVAR(logVerbose))
		{
			std::cout << "touching cached object: " << signature << std::endl;
			indenter = boost::in_place();
		}

		auto iter(pool.find(signature));
		if (iter != pool.end())
		{
			auto &datum(iter->second);
			datum.atim = time;
		}
	}

	void Cache::Invalidate(const Signature &signature)
	{
		// write to the log
		boost::optional<log::Indenter> indenter;
		if (*CVAR(logCache) && *CVAR(logCacheUpdate) && *CVAR(logVerbose))
		{
			std::cout << "invalidating cached object: " << signature << std::endl;
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

	void Cache::Purge(const Signature &signature)
	{
		// write to the log
		boost::optional<log::Indenter> indenter;
		if (*CVAR(logCache))
		{
			std::cout << "purging cached object: " << signature << std::endl;
			indenter = boost::in_place();
		}

		pool.erase(signature);
	}

	void Cache::PurgeResource(const std::string &path)
	{
		// FIXME: implement
	}

	/*----------------+
	| other functions |
	+----------------*/

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
					std::cout << "cached object timed out: " << iter->first << std::endl;
					indenter = boost::in_place();
				}

				pool.erase(iter++);
			}
			else ++iter;
		}
	}
}}
