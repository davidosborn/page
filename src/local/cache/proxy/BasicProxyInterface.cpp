#include "../Cache.hpp" // Cache::{GetGlobalInstance,Invalidate,Purge,Touch}
#include "BasicProxyInterface.hpp"

namespace page { namespace cache
{
	/*-----------------+
	| cache operations |
	+-----------------*/

	std::function<void ()> BasicProxyInterface::GetTouchFunction(const Signature &signature) const
	{
		return std::bind(&Cache::Touch, Cache::GetGlobalInstance(), signature);
	}

	std::function<void ()> BasicProxyInterface::GetInvalidateFunction(const Signature &signature) const
	{
		return std::bind(&Cache::Invalidate, Cache::GetGlobalInstance(), signature);
	}

	std::function<void ()> BasicProxyInterface::GetPurgeFunction(const Signature &signature) const
	{
		return std::bind(
			static_cast<void (Cache::*)(const Signature &)>(&Cache::Purge),
			Cache::GetGlobalInstance(), signature);
	}
}}
