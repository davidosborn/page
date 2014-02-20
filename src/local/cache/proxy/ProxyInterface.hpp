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

#ifndef    page_local_cache_proxy_ProxyInterface_hpp
#   define page_local_cache_proxy_ProxyInterface_hpp

#	include <iosfwd> // basic_ostream
#	include <memory> // shared_ptr

#	include "BasicProxyInterface.hpp"

namespace page { namespace cache
{
	class Signature;

	/**
	 * The interface for proxies of cached objects.
	 *
	 * @note Uses the "Curiously-Recurring Template" and "Non-Virtual Interface"
	 *       patterns.  The derived class must implement DoLock() and
	 *       DoGetSignature().
	 */
	template <typename Derived, typename T>
		class ProxyInterface : public BasicProxyInterface
	{
		/*-------+
		| traits |
		+-------*/

		public:
		/**
		 * The type of the cached object.
		 */
		using element_type = const T;

		/**
		 * The type of a shared reference to the cached object.
		 */
		using pointer = std::shared_ptr<element_type>;

		/*----------+
		| observers |
		+----------*/

		/**
		 * @return *lock().
		 */
		element_type &operator *() const;

		/**
		 * @return lock().
		 */
		pointer operator ->() const;

		/**
		 * @return lock().
		 */
		pointer get() const;

		/**
		 * @return Derived::DoLock().
		 */
		pointer lock() const;

		/**
		 * @return !GetSignature().empty().
		 */
		explicit operator bool() const noexcept;

		/**
		 * @return Derived::DoGetSignature().
		 */
		const Signature &GetSignature() const noexcept;

		/*-----------------+
		| cache operations |
		+-----------------*/

		/**
		 * @return A function that will touch the cached object.
		 */
		std::function<void ()> GetTouchFunction() const;

		/**
		 * @return A function that will invalidate the cached object.
		 */
		std::function<void ()> GetInvalidateFunction() const;

		/**
		 * @return A function that will purge the cached object.
		 */
		std::function<void ()> GetPurgeFunction() const;
	};

	/*---------------------+
	| signature comparison |
	+---------------------*/

	template <
		typename D, typename T,
		typename E, typename U>
		bool operator ==(
			const ProxyInterface<D, T> &,
			const ProxyInterface<E, U> &) noexcept;

	template <
		typename D, typename T,
		typename E, typename U>
		bool operator !=(
			const ProxyInterface<D, T> &,
			const ProxyInterface<E, U> &) noexcept;

	template <
		typename D, typename T,
		typename E, typename U>
		bool operator <(
			const ProxyInterface<D, T> &,
			const ProxyInterface<E, U> &) noexcept;

	template <
		typename D, typename T,
		typename E, typename U>
		bool operator >(
			const ProxyInterface<D, T> &,
			const ProxyInterface<E, U> &) noexcept;

	template <
		typename D, typename T,
		typename E, typename U>
		bool operator <=(
			const ProxyInterface<D, T> &,
			const ProxyInterface<E, U> &) noexcept;

	template <
		typename D, typename T,
		typename E, typename U>
		bool operator >=(
			const ProxyInterface<D, T> &,
			const ProxyInterface<E, U> &) noexcept;

	/*-------------------+
	| nullptr comparison |
	+-------------------*/

	template <typename D, typename T>
		bool operator ==(const ProxyInterface<D, T> &, std::nullptr_t) noexcept;

	template <typename D, typename T>
		bool operator ==(std::nullptr_t, const ProxyInterface<D, T> &) noexcept;

	template <typename D, typename T>
		bool operator !=(const ProxyInterface<D, T> &, std::nullptr_t) noexcept;

	template <typename D, typename T>
		bool operator !=(std::nullptr_t, const ProxyInterface<D, T> &) noexcept;

	/*-----------------+
	| stream insertion |
	+-----------------*/

	/**
	 * Writes the proxy's signature to the output stream.
	 */
	template <typename Char, typename CharTraits, typename D, typename T>
		std::basic_ostream<Char, CharTraits> &operator <<(
			std::basic_ostream<Char, CharTraits> &, const ProxyInterface<D, T> &);
}}

////////// std::hash<ProxyInterface> ///////////////////////////////////////////

namespace std
{
	template <typename>
		struct hash;

	/**
	 * A specialization of std::hash for ::page::cache::ProxyInterface.
	 */
	template <typename D, typename T>
		struct hash<::page::cache::ProxyInterface<D, T>>
	{
		using result_type   = typename hash<string>::result_type;
		using argument_type = ::page::cache::ProxyInterface<D, T>;

		result_type operator ()(const argument_type &) const noexcept;
	};
}

#	include "ProxyInterface.tpp"
#endif
