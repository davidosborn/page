#ifndef    page_local_cache_proxy_BasicProxy_hpp
#   define page_local_cache_proxy_BasicProxy_hpp

#	include "../../util/class/Cloneable.hpp"
#	include "ProxyInterface.hpp"

namespace page { namespace cache
{
	/**
	 * A base class for the implementations of proxies of cached objects.
	 */
	template <typename T>
		class BasicProxy :
			public ProxyInterface<BasicProxy<T>, T>,
			public virtual util::Cloneable<BasicProxy<T>>
	{
		/*-------+
		| traits |
		+-------*/

		public:
		using ProxyInterface<BasicProxy<T>, T>::pointer;

		/*-------------+
		| constructors |
		+-------------*/

		protected:
		BasicProxy() = default;
		explicit BasicProxy(const std::string &signature);

		/*------------------------------+
		| ProxyInterface implementation |
		+------------------------------*/

		private:
		virtual pointer DoLock() const = 0;
		const std::string &DoGetSignature() const noexcept;

		/*-------------+
		| data members |
		+-------------*/

		/**
		 * A signature that uniquely identifies the resource.
		 */
		std::string signature;
	};
}}

#	include "BasicProxy.tpp"
#endif
