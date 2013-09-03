#ifndef    page_local_cache_proxy_BasicProxy_hpp
#   define page_local_cache_proxy_BasicProxy_hpp

#	include "../../util/class/Cloneable.hpp"
#	include "../Signature.hpp"
#	include "ProxyInterface.hpp"

namespace page { namespace cache
{
	template <typename T>
		class Proxy;

	/**
	 * A base class for the implementations of proxies of cached objects.
	 */
	template <typename T>
		class BasicProxy :
			public ProxyInterface<BasicProxy<T>, T>,
			public virtual util::Cloneable<BasicProxy<T>>
	{
		friend class Proxy<T>;
		friend class ProxyInterface<BasicProxy<T>, T>;

		/*-------+
		| traits |
		+-------*/

		public:
		using typename ProxyInterface<BasicProxy<T>, T>::pointer;

		/*-------------+
		| constructors |
		+-------------*/

		protected:
		BasicProxy() = default;
		explicit BasicProxy(const Signature &signature);

		/*------------------------------+
		| ProxyInterface implementation |
		+------------------------------*/

		private:
		virtual pointer DoLock() const = 0;
		const Signature &DoGetSignature() const noexcept;

		/*-------------+
		| data members |
		+-------------*/

		/**
		 * A signature that uniquely identifies the resource.
		 */
		Signature signature;
	};
}}

#	include "BasicProxy.tpp"
#endif
