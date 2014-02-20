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

#include <ostream> // basic_ostream

#include "../../err/Exception.hpp"
#include "../Signature.hpp"

namespace page { namespace cache
{
	template <typename D, typename T>
		auto ProxyInterface<D, T>::operator *() const -> element_type &
	{
		return *lock();
	}

	template <typename D, typename T>
		auto ProxyInterface<D, T>::operator ->() const -> pointer
	{
		return lock();
	}

	template <typename D, typename T>
		auto ProxyInterface<D, T>::get() const -> pointer
	{
		return lock();
	}

	template <typename D, typename T>
		auto ProxyInterface<D, T>::lock() const -> pointer
	{
		auto r(static_cast<const D &>(*this).DoLock());
		if (r == nullptr)
			THROW(std::bad_weak_ptr());
		return r;
	}

	template <typename D, typename T>
		ProxyInterface<D, T>::operator bool() const noexcept
	{
		return GetSignature() != nullptr;
	}

	template <typename D, typename T>
		const Signature &ProxyInterface<D, T>::GetSignature() const noexcept
	{
		return static_cast<const D &>(*this).DoGetSignature();
	}

	/*-----------------+
	| cache operations |
	+-----------------*/

	template <typename D, typename T>
		std::function<void ()> ProxyInterface<D, T>::GetTouchFunction() const
	{
		return BasicProxyInterface::GetTouchFunction(GetSignature());
	}

	template <typename D, typename T>
		std::function<void ()> ProxyInterface<D, T>::GetInvalidateFunction() const
	{
		return BasicProxyInterface::GetInvalidateFunction(GetSignature());
	}

	template <typename D, typename T>
		std::function<void ()> ProxyInterface<D, T>::GetPurgeFunction() const
	{
		return BasicProxyInterface::GetPurgeFunction(GetSignature());
	}

	/*---------------------+
	| signature comparison |
	+---------------------*/

	template <
		typename D, typename T,
		typename E, typename U>
		bool operator ==(
			const ProxyInterface<D, T> &a,
			const ProxyInterface<E, U> &b) noexcept
	{
		return a.GetSignature() == b.GetSignature();
	}

	template <
		typename D, typename T,
		typename E, typename U>
		bool operator !=(
			const ProxyInterface<D, T> &a,
			const ProxyInterface<E, U> &b) noexcept
	{
		return a.GetSignature() != b.GetSignature();
	}

	template <
		typename D, typename T,
		typename E, typename U>
		bool operator <(
			const ProxyInterface<D, T> &a,
			const ProxyInterface<E, U> &b) noexcept
	{
		return a.GetSignature() < b.GetSignature();
	}

	template <
		typename D, typename T,
		typename E, typename U>
		bool operator >(
			const ProxyInterface<D, T> &a,
			const ProxyInterface<E, U> &b) noexcept
	{
		return a.GetSignature() > b.GetSignature();
	}

	template <
		typename D, typename T,
		typename E, typename U>
		bool operator <=(
			const ProxyInterface<D, T> &a,
			const ProxyInterface<E, U> &b) noexcept
	{
		return a.GetSignature() <= b.GetSignature();
	}

	template <
		typename D, typename T,
		typename E, typename U>
		bool operator >=(
			const ProxyInterface<D, T> &a,
			const ProxyInterface<E, U> &b) noexcept
	{
		return a.GetSignature() >= b.GetSignature();
	}

	/*-------------------+
	| nullptr comparison |
	+-------------------*/

	template <typename D, typename T>
		bool operator ==(const ProxyInterface<D, T> &p, std::nullptr_t) noexcept
	{
		return !p;
	}

	template <typename D, typename T>
		bool operator ==(std::nullptr_t, const ProxyInterface<D, T> &p) noexcept
	{
		return !p;
	}

	template <typename D, typename T>
		bool operator !=(const ProxyInterface<D, T> &p, std::nullptr_t) noexcept
	{
		return p;
	}

	template <typename D, typename T>
		bool operator !=(std::nullptr_t, const ProxyInterface<D, T> &p) noexcept
	{
		return p;
	}

	/*-----------------+
	| stream insertion |
	+-----------------*/

	template <typename Char, typename CharTraits, typename D, typename T>
		std::basic_ostream<Char, CharTraits> &operator <<(
			std::basic_ostream<Char, CharTraits> &os, const ProxyInterface<D, T> &proxy)
	{
		return os << proxy.GetSignature();
	}
}}

////////// std::hash<ProxyInterface> ///////////////////////////////////////////

namespace std
{
	template <typename D, typename T>
		auto hash<::page::cache::ProxyInterface<D, T>>::operator ()(const argument_type &proxy) const noexcept -> result_type
	{
		return hash<string>()(proxy.GetSignature());
	}
}
