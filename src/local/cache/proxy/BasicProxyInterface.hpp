#ifndef    page_local_cache_proxy_BasicProxyInterface_hpp
#   define page_local_cache_proxy_BasicProxyInterface_hpp

#	include <functional> // function

namespace page { namespace cache
{
	class Signature;

	/**
	 * Provides a template-independent implementation of some member functions
	 * in ProxyInterface.
	 */
	class BasicProxyInterface
	{
		/*-----------------+
		| cache operations |
		+-----------------*/

		protected:
		/**
		 * @return A function that will touch the cached object.
		 */
		std::function<void ()> GetTouchFunction(const Signature &) const;

		/**
		 * @return A function that will invalidate the cached object.
		 */
		std::function<void ()> GetInvalidateFunction(const Signature &) const;

		/**
		 * @return A function that will purge the cached object.
		 */
		std::function<void ()> GetPurgeFunction(const Signature &) const;
	};
}}

#endif
