/**
 * @copyright
 *
 * Copyright (c) 2006-2014 David Osborn
 *
 * Permission is granted to use and redistribute this software in source and
 * binary form, with or without modification, subject to the following
 * conditions:
 *
 * 1. Redistributions in source form must retain the above copyright notice,
 *    this list of conditions, and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions, and the following disclaimer in the same place
 *    and form as other copyright, license, and disclaimer information.
 *
 * 3. Redistributions in binary form must also include an acknowledgement in the
 *    same place and form as other acknowledgements (such as the credits),
 *    similar in substance to the following:
 *
 *       Portions of this software are based on the work of David Osborn.
 *
 * This software is provided "as is", without any express or implied warranty.
 * In no event will the authors be liable for any damages arising out of the use
 * of this software.
 */

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
			public util::Cloneable<BasicProxy<T>>
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
