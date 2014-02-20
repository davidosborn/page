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

#include "../../err/Exception.hpp"

namespace page { namespace util
{
	/*-------------+
	| constructors |
	+-------------*/

	template <typename T>
		factory_ptr<T>::factory_ptr(std::nullptr_t) noexcept {}

	template <typename T>
		factory_ptr<T>::factory_ptr(const factory_function &f) :
			f(f) {}

	template <typename T> template <typename U>
		factory_ptr<T>::factory_ptr(const factory_ptr<U> &other) :
			f(other.f) {}

	/*----------+
	| observers |
	+----------*/

	template <typename T>
		auto factory_ptr<T>::operator *() const -> element_type &
	{
		assert(*this);
		return *lock();
	}

	template <typename T>
		auto factory_ptr<T>::operator ->() const -> pointer
	{
		return lock();
	}

	template <typename T>
		auto factory_ptr<T>::get() const -> pointer
	{
		return lock();
	}

	template <typename T>
		auto factory_ptr<T>::lock() const -> pointer
	{
		if (*this) return f();
		THROW(std::bad_weak_ptr());
	}

	template <typename T>
		factory_ptr<T>::operator bool() const noexcept
	{
		return f;
	}
}}
