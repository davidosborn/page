/**
 * @section license
 *
 * Copyright (c) 2006-2012 David Osborn
 *
 * Permission is granted to use and redistribute this software in source and
 * binary form, with or without modification, subject to the following
 * conditions:
 *
 * 1. Redistributions in source form must retain the above copyright notice,
 *    this list of conditions, and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions, and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution, and in the same
 *    place and form as other copyright, license, and disclaimer information.
 *
 * As a special exception, distributions of derivative works in binary form may
 * include an acknowledgement in place of the above copyright notice, this list
 * of conditions, and the following disclaimer in the documentation and/or other
 * materials provided with the distribution, and in the same place and form as
 * other acknowledgements, similar in substance to the following:
 *
 *    Portions of this software are based on the work of David Osborn.
 *
 * This software is provided "as is", without any express or implied warranty.
 * In no event will the authors be liable for any damages arising out of the use
 * of this software.
 */

#include <cassert>

namespace page
{
	namespace util
	{
		// constructors
		template <typename T> Optional<T>::Optional() :
			empty(true) {}
		template <typename T> Optional<T>::Optional(const T &value) :
			empty(false)
		{
			new (&storage) T(value);
		}
		template <typename T> Optional<T>::Optional(std::nullptr_t) :
			empty(true) {}
		template <typename T> Optional<T>::Optional(const Optional &other) :
			empty(other.empty)
		{
			if (!empty) new (&storage) T(*other);
		}
		template <typename T> template <typename U> Optional<T>::Optional(const Optional<U> &other) :
			empty(other.empty)
		{
			if (!empty) new (&storage) T(*other);
		}
		
		// destructor
		template <typename T> Optional<T>::~Optional()
		{
			if (!empty) (*this)->~T();
		}
		
		// assignment
		template <typename T> Optional<T> &Optional<T>::operator =(const T &value)
		{
			if (empty)
			{
				new (&storage) T(value);
				empty = false;
			}
			else **this = value;
			return *this;
		}
		template <typename T> Optional<T> &Optional<T>::operator =(const Optional &other)
		{
			if (!other.empty)
			{
				if (empty)
				{
					new (&storage) T(*other);
					empty = false;
				}
				else **this = *other;
			}
			else if (!empty)
			{
				(*this)->~T();
				empty = true;
			}
			return *this;
		}
		template <typename T> template <typename U> Optional<T> &Optional<T>::operator =(const Optional<U> &other)
		{
			if (!other.empty)
			{
				if (empty)
				{
					new (&storage) T(*other);
					empty = false;
				}
				else **this = *other;
			}
			else if (!empty)
			{
				(*this)->~T();
				empty = true;
			}
			return *this;
		}

		// conversion
		template <typename T> template <typename U> Optional<T>::operator Optional<U>() const
		{
			return Optional<U>(*this);
		}

		// observers
		template <typename T> T &Optional<T>::operator *() noexcept
		{
			assert(!empty);
			return reinterpret_cast<T &>(storage);
		}
		template <typename T> const T &Optional<T>::operator *() const noexcept
		{
			assert(!empty);
			return reinterpret_cast<const T &>(storage);
		}
		template <typename T> T *Optional<T>::operator ->() noexcept
		{
			assert(!empty);
			return reinterpret_cast<T *>(&storage);
		}
		template <typename T> const T *Optional<T>::operator ->() const noexcept
		{
			assert(!empty);
			return reinterpret_cast<const T *>(&storage);
		}
		template <typename T> T *Optional<T>::get() noexcept
		{
			return !empty ? reinterpret_cast<T *>(&storage) : static_cast<T *>(nullptr);
		}
		template <typename T> const T *Optional<T>::get() const noexcept
		{
			return !empty ? reinterpret_cast<const T *>(&storage) : static_cast<const T *>(nullptr);
		}
		template <typename T> Optional<T>::operator bool() const noexcept
		{
			return !empty;
		}

		// modifiers
		template <typename T> void Optional<T>::reset()
		{
			if (!empty)
			{
				(*this)->~T();
				empty = true;
			}
		}
		template <typename T> void Optional<T>::reset(const T &value)
		{
			*this = value;
		}
		template <typename T> void Optional<T>::reset(std::nullptr_t)
		{
			reset();
		}
		template <typename T> void Optional<T>::swap(Optional &other) noexcept
		{
			using std::swap;
			swap(empty,   other.empty);
			swap(storage, other.storage);
		}

		// relational operators
		template <typename T, typename U> bool operator < (const Optional<T> &a, const Optional<U> &b) noexcept { return a && b ? *a <  *b : !!a <  !!b; }
		template <typename T, typename U> bool operator > (const Optional<T> &a, const Optional<U> &b) noexcept { return a && b ? *a >  *b : !!a >  !!b; }
		template <typename T, typename U> bool operator <=(const Optional<T> &a, const Optional<U> &b) noexcept { return a && b ? *a <= *b : !!a <= !!b; }
		template <typename T, typename U> bool operator >=(const Optional<T> &a, const Optional<U> &b) noexcept { return a && b ? *a >= *b : !!a >= !!b; }
		template <typename T, typename U> bool operator ==(const Optional<T> &a, const Optional<U> &b) noexcept { return a && b ? *a == *b : !!a == !!b; }
		template <typename T, typename U> bool operator !=(const Optional<T> &a, const Optional<U> &b) noexcept { return a && b ? *a != *b : !!a != !!b; }
		
		// nullptr relational operators
		template <typename T> bool operator ==(const Optional<T> &a, std::nullptr_t) noexcept { return  !a; }
		template <typename T> bool operator ==(std::nullptr_t, const Optional<T> &a) noexcept { return  !a; }
		template <typename T> bool operator !=(const Optional<T> &a, std::nullptr_t) noexcept { return !!a; }
		template <typename T> bool operator !=(std::nullptr_t, const Optional<T> &a) noexcept { return !!a; }

		// specialized algorithms
		template <typename T> void swap(Optional<T> &a, Optional<T> &b) noexcept
		{
			a.swap(b);
		}
	}
}
