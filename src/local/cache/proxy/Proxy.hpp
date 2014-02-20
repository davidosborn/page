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
