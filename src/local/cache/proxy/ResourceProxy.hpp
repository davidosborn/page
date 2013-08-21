#ifndef    page_local_cache_proxy_ResourceProxy_hpp
#   define page_local_cache_proxy_ResourceProxy_hpp

#	include <string>

#	include "BasicProxy.hpp"

namespace page { namespace cache
{
	/**
	 * A proxy representing a cached resource.
	 */
	template <typename T>
		class ResourceProxy :
			public BasicProxy<T>,
			public virtual util::Cloneable<ResourceProxy<T>, BasicProxy<T>>
	{
		/*-------+
		| traits |
		+-------*/

		public:
		using typename BasicProxy<T>::pointer;

		/*-------------+
		| constructors |
		+-------------*/

		explicit ResourceProxy(const std::string &path = "");

		/*--------------------------+
		| BasicProxy implementation |
		+--------------------------*/

		private:
		pointer DoLock() const override;

		/*-------------+
		| data members |
		+-------------*/

		std::string path;
	};
}}

#	include "ResourceProxy.tpp"
#endif
