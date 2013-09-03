#ifndef    page_local_cache_proxy_Proxy_hpp
#   define page_local_cache_proxy_Proxy_hpp

#	include <cstddef> // nullptr_t

#	include "ProxyInterface.hpp"

namespace page { namespace cache
{
	template <typename T>
		class BasicProxy;

	/**
	 * A generic adaptor for proxies of cached objects, which provides value
	 * semantics.
	 */
	template <typename T>
		class Proxy : public ProxyInterface<Proxy<T>, T>
	{
		friend class ProxyInterface<Proxy<T>, T>;

		/*-------+
		| traits |
		+-------*/

		public:
		using typename ProxyInterface<Proxy<T>, T>::pointer;

		/*-------------+
		| constructors |
		+-------------*/

		Proxy() = default;
		Proxy(std::nullptr_t);
		Proxy(const BasicProxy<T> &);

		/*------------------------------+
		| ProxyInterface implementation |
		+------------------------------*/

		private:
		pointer DoLock() const;
		const Signature &DoGetSignature() const noexcept;

		/*-------------+
		| data members |
		+-------------*/

		/**
		 * The wrapped proxy, which forms the concrete implementation.
		 */
		std::shared_ptr<BasicProxy<T>> impl;
	};
}}

#	include "Proxy.tpp"
#endif
