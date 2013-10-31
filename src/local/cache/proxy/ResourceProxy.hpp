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
		class ResourceProxy : public BasicProxy<T>
	{
		IMPLEMENT_CLONEABLE(ResourceProxy, BasicProxy<T>)

		/*-------+
		| traits |
		+-------*/

		public:
		using typename BasicProxy<T>::pointer;

		/*-------------+
		| constructors |
		+-------------*/

		ResourceProxy() = default;
		ResourceProxy(std::nullptr_t);
		explicit ResourceProxy(const std::string &path);

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
