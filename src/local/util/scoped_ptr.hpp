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

#ifndef    page_local_util_scoped_ptr_hpp
#   define page_local_util_scoped_ptr_hpp

#	include <cstddef> // nullptr_t
#	include <functional> // function
#	include <iosfwd> // ostream
#	include <memory> // {auto,unique}_ptr
#	include "NonCopyable.hpp"

namespace page
{
	namespace util
	{
		/**
		 * A scoped pointer with a custom deleter.
		 *
		 * @note This class supports incomplete types.
		 * @note The deleter may be initialized to @c nullptr on construction,
		 *       in which case it will not be executed on destruction.
		 *
		 * @ingroup smart-pointer
		 */
		template <typename T> struct scoped_ptr : NonCopyable
		{
			typedef T *pointer;
			typedef T element_type;
			typedef std::function<void (T *)> deleter_type;
			typedef std::function<void (const T *)> const_deleter_type;

			// constructors
			scoped_ptr();
			explicit scoped_ptr(pointer);
			scoped_ptr(pointer, deleter_type);
			scoped_ptr(std::nullptr_t);
			scoped_ptr(std::nullptr_t, deleter_type);
			template <typename U> scoped_ptr(std::auto_ptr<U> &&);
			template <typename U, typename D> scoped_ptr(std::unique_ptr<U, D> &&);

			// destructor
			~scoped_ptr();

			// assignment
			template <typename U> scoped_ptr &operator =(std::auto_ptr<U> &&);
			template <typename U, typename D> scoped_ptr &operator =(std::unique_ptr<U, D> &&);

			// observers
			T &operator *() const noexcept;
			pointer operator ->() const noexcept;
			pointer get() const noexcept;
			const deleter_type &get_deleter() const noexcept;
			const_deleter_type get_const_deleter() const;
			explicit operator bool() const noexcept;

			// modifiers
			pointer release() noexcept;
			void reset();
			template <typename U> void reset(U *);
			template <typename U, typename D> void reset(U *, D);
			void reset(std::nullptr_t);
			template <typename D> void reset(std::nullptr_t, D);
			void swap(scoped_ptr &) noexcept;

			private:
			class DefaultDeleter;
			class ConstDeleterAdapter;

			pointer p;
			deleter_type deleter;
		};

		// relational operators
		template <typename T, typename U> bool operator < (const scoped_ptr<T> &, const scoped_ptr<U> &) noexcept;
		template <typename T, typename U> bool operator > (const scoped_ptr<T> &, const scoped_ptr<U> &) noexcept;
		template <typename T, typename U> bool operator <=(const scoped_ptr<T> &, const scoped_ptr<U> &) noexcept;
		template <typename T, typename U> bool operator >=(const scoped_ptr<T> &, const scoped_ptr<U> &) noexcept;
		template <typename T, typename U> bool operator ==(const scoped_ptr<T> &, const scoped_ptr<U> &) noexcept;
		template <typename T, typename U> bool operator !=(const scoped_ptr<T> &, const scoped_ptr<U> &) noexcept;

		// nullptr relational operators
		template <typename T> bool operator ==(const scoped_ptr<T> &a, std::nullptr_t) noexcept;
		template <typename T> bool operator ==(std::nullptr_t, const scoped_ptr<T> &a) noexcept;
		template <typename T> bool operator !=(const scoped_ptr<T> &a, std::nullptr_t) noexcept;
		template <typename T> bool operator !=(std::nullptr_t, const scoped_ptr<T> &a) noexcept;

		// stream insertion
		template <typename T, typename Char, typename CharTraits>
			std::basic_ostream<Char, CharTraits> &operator <<(std::basic_ostream<Char, CharTraits> &, const scoped_ptr<T> &) noexcept;

		// specialized algorithms
		template <typename T> void swap(scoped_ptr<T> &, scoped_ptr<T> &) noexcept;
	}
}

#	include "scoped_ptr.tpp"
#endif
