/**
 * @section license
 *
 * Copyright (c) 2006-2013 David Osborn
 *
 * Permission is granted to use and redistribute this software in source and
 * binary form, with or without modification, subject to the following
 * conditions:
 *
 * 1. Redistributions in source form must retain the above copyright notice,
 *    this list of conditions, and the following disclaimer.
 *
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
#include <ostream> // basic_ostream
#include <utility> // forward, move

#include "../algorithm/downcast.hpp"

namespace page { namespace util
{
	namespace detail
	{
		/**
		 * The default implementation of a cloner.
		 */
		template <typename T>
			struct DefaultCloner
		{
			typedef std::unique_ptr<T> result_type;
			typedef const T &argument_type;

			std::unique_ptr<T> operator ()(const T &x) const
			{
				return std::unique_ptr<T>(new T(x));
			}
		};

		/**
		 * A null cloner.
		 */
		template <typename T>
			struct NullCloner
		{
			typedef std::unique_ptr<T> result_type;
			typedef const T &argument_type;

			std::unique_ptr<T> operator ()(const T &) const
			{
				return nullptr;
			}
		};

		/**
		 * An adaptor for cloners to support implicit pointer conversion, such as
		 * derived-to-base-pointer conversion.
		 */
		template <typename T, typename U>
			struct ClonerAdaptor
		{
			typedef std::unique_ptr<T> result_type;
			typedef const T &argument_type;

			private:
			typedef std::function<std::unique_ptr<U> (const U &)> cloner_type;

			public:
			explicit ClonerAdaptor(const cloner_type &cloner) :
				cloner(cloner) {}

			std::unique_ptr<T> operator ()(const T &x) const
			{
				return cloner(downcast<const U &>(x));
			}

			private:
			cloner_type cloner;
		};
	}

////////// copy_ptr ////////////////////////////////////////////////////////////

	/*-------------+
	| constructors |
	+-------------*/

	template <typename T>
		constexpr copy_ptr<T>::copy_ptr() :
			copy_ptr(nullptr) {}

	template <typename T>
		constexpr copy_ptr<T>::copy_ptr(std::nullptr_t) :
			copy_ptr(static_cast<T *>(nullptr), detail::NullCloner<T>()) {}

	template <typename T>
		copy_ptr<T>::copy_ptr(T *p) :
			copy_ptr(p, detail::DefaultCloner<T>()) {}

	template <typename T>
		copy_ptr<T>::copy_ptr(T *p, const cloner_type &cloner) :
			p(p), cloner(cloner)
	{
		assert(cloner != nullptr);
	}

	/*------------------------+
	| unique_ptr constructors |
	+------------------------*/

	template <typename T>
		copy_ptr<T>::copy_ptr(std::unique_ptr<T> &&other) :
			copy_ptr(std::forward<std::unique_ptr<T>>(other), detail::DefaultCloner<T>()) {}

	template <typename T>
		copy_ptr<T>::copy_ptr(std::unique_ptr<T> &&other, const cloner_type &cloner) :
			p(std::move(other)), cloner(cloner)
	{
		assert(cloner != nullptr);
	}

	template <typename T> template <typename U>
		copy_ptr<T>::copy_ptr(std::unique_ptr<U> &&other) :
			copy_ptr(std::forward<std::unique_ptr<U>>(other), detail::DefaultCloner<T>()) {}

	template <typename T> template <typename U>
		copy_ptr<T>::copy_ptr(std::unique_ptr<U> &&other, const cloner_type &cloner) :
			p(std::move(other)), cloner(cloner)
	{
		assert(cloner != nullptr);
	}

	/*---------------+
	| copy semantics |
	+---------------*/

	template <typename T>
		copy_ptr<T>::copy_ptr(const copy_ptr &other) :
			p(other.cloner(*other)), cloner(other.cloner) {}

	template <typename T>
		copy_ptr<T> &copy_ptr<T>::operator =(const copy_ptr &other)
	{
		p = other.cloner(*other);
		cloner = other.cloner;
		return *this;
	}

	template <typename T> template <typename U>
		copy_ptr<T>::copy_ptr(const copy_ptr<U> &other) :
			p(other.cloner(*other)),
			cloner(detail::ClonerAdaptor<T, U>(other.cloner)) {}

	template <typename T> template <typename U>
		copy_ptr<T> &copy_ptr<T>::operator =(const copy_ptr<U> &other)
	{
		p = other.cloner(*other);
		cloner = detail::ClonerAdaptor<T, U>(other.cloner);
		return *this;
	}

	/*---------------+
	| move semantics |
	+---------------*/

	template <typename T>
		copy_ptr<T>::copy_ptr(copy_ptr &&other) :
			p(std::move(other.p)),
			cloner(std::move(other.cloner)) {}

	template <typename T>
		copy_ptr<T> &copy_ptr<T>::operator =(copy_ptr &&other)
	{
		p = std::move(other.p);
		cloner = std::move(other.cloner);
		return *this;
	}

	template <typename T> template <typename U>
		copy_ptr<T>::copy_ptr(copy_ptr<U> &&other) :
			p(std::move(other.p)),
			cloner(detail::ClonerAdaptor<T, U>(other.cloner)) {}

	template <typename T> template <typename U>
		copy_ptr<T> &copy_ptr<T>::operator =(copy_ptr<U> &&other)
	{
		p = std::move(other.p);
		cloner = detail::ClonerAdaptor<T, U>(other.cloner);
		return *this;
	}

	/*----------+
	| observers |
	+----------*/

	template <typename T>
		T &copy_ptr<T>::operator *() const noexcept
	{
		return *p;
	}

	template <typename T>
		const typename copy_ptr<T>::pointer &copy_ptr<T>::operator ->() const noexcept
	{
		return p;
	}

	template <typename T>
		T *copy_ptr<T>::get() const noexcept
	{
		return p.get();
	}

	template <typename T>
		typename copy_ptr<T>::deleter_type &copy_ptr<T>::get_deleter() noexcept
	{
		return p.get_deleter();
	}

	template <typename T>
		const typename copy_ptr<T>::deleter_type &copy_ptr<T>::get_deleter() const noexcept
	{
		return p.get_deleter();
	}

	template <typename T>
		typename copy_ptr<T>::cloner_type &copy_ptr<T>::get_cloner() noexcept
	{
		return cloner;
	}

	template <typename T>
		const typename copy_ptr<T>::cloner_type &copy_ptr<T>::get_cloner() const noexcept
	{
		return cloner;
	}

	template <typename T>
		copy_ptr<T>::operator bool() const noexcept
	{
		return p != nullptr;
	}

	/*----------+
	| modifiers |
	+----------*/

	template <typename T>
		T *copy_ptr<T>::release() noexcept
	{
		cloner = detail::NullCloner<T>();
		return p.release();
	}

	template <typename T>
		void copy_ptr<T>::reset(T *p)
	{
		swap(copy_ptr(p));
	}

	template <typename T>
		void copy_ptr<T>::reset(T *p, const cloner_type &cloner)
	{
		swap(copy_ptr(p, cloner));
	}

	template <typename T>
		void copy_ptr<T>::swap(copy_ptr &other)
	{
		using std::swap;
		swap(p,      other.p);
		swap(cloner, other.cloner);
	}

	/*-------------------+
	| pointer comparison |
	+-------------------*/

	template <typename T, typename U>
		bool operator ==(const copy_ptr<T> &a, const copy_ptr<U> &b) noexcept
	{
		return a.get() == b.get();
	}

	template <typename T, typename U>
		bool operator !=(const copy_ptr<T> &a, const copy_ptr<U> &b) noexcept
	{
		return a.get() != b.get();
	}

	template <typename T, typename U>
		bool operator <=(const copy_ptr<T> &a, const copy_ptr<U> &b) noexcept
	{
		return a.get() <= b.get();
	}

	template <typename T, typename U>
		bool operator >=(const copy_ptr<T> &a, const copy_ptr<U> &b) noexcept
	{
		return a.get() >= b.get();
	}

	template <typename T, typename U>
		bool operator <(const copy_ptr<T> &a, const copy_ptr<U> &b) noexcept
	{
		return a.get() < b.get();
	}

	template <typename T, typename U>
		bool operator >(const copy_ptr<T> &a, const copy_ptr<U> &b) noexcept
	{
		return a.get() > b.get();
	}

	/*-------------------+
	| nullptr comparison |
	+-------------------*/

	template <typename T>
		bool operator ==(const copy_ptr<T> &p, std::nullptr_t) noexcept
	{
		return p.get() == nullptr;
	}

	template <typename T>
		bool operator ==(std::nullptr_t, const copy_ptr<T> &p) noexcept
	{
		return p.get() == nullptr;
	}

	template <typename T>
		bool operator !=(const copy_ptr<T> &p, std::nullptr_t) noexcept
	{
		return p.get() != nullptr;
	}

	template <typename T>
		bool operator !=(std::nullptr_t, const copy_ptr<T> &p) noexcept
	{
		return p.get() != nullptr;
	}

	/*-----------------+
	| stream insertion |
	+-----------------*/

	template <typename Char, typename CharTraits, typename T>
		std::basic_ostream<Char, CharTraits> &operator <<(
			std::basic_ostream<Char, CharTraits> &os, const copy_ptr<T> &p)
	{
		return os << *p;
	}
}}

namespace std
{
	template <typename T, typename U>
		void swap(::page::util::copy_ptr<T> &a, ::page::util::copy_ptr<U> &b)
	{
		a.swap(b);
	}
}
