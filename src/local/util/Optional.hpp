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

#ifndef    page_local_util_Optional_hpp
#   define page_local_util_Optional_hpp

#	include <cstddef> // nullptr_t
#	include <type_traits> // aligned_storage, alignment_of

namespace page
{
	namespace util
	{
		/**
		 * Optional value wrapper with pointer semantics.
		 *
		 * @note Inspired by Fernando Luis Cacciola Carballal's boost/optional
		 *       library.
		 *
		 * @ingroup smart-pointer
		 */
		template <typename T> struct Optional
		{
			// constructors
			Optional();
			Optional(const T &);
			Optional(std::nullptr_t);
			Optional(const Optional &);
			template <typename U> explicit Optional(const Optional<U> &);
			
			// destructor
			~Optional();
			
			// assignment
			Optional &operator =(const T &);
			Optional &operator =(const Optional &);
			template <typename U> Optional &operator =(const Optional<U> &);

			// conversion
			template <typename U> operator Optional<U>() const;

			// observers
			T &operator  *() noexcept; const T &operator  *() const noexcept;
			T *operator ->() noexcept; const T *operator ->() const noexcept;
			T *get()         noexcept; const T *get()         const noexcept;
			explicit operator bool() const noexcept;

			// modifiers
			void reset();
			void reset(const T &);
			void reset(std::nullptr_t);
			void swap(Optional &) noexcept;

			private:
			bool empty;
			typename std::aligned_storage<sizeof(T), std::alignment_of<T>::value>::type storage;
		};

		// relational operators
		template <typename T, typename U> bool operator < (const Optional<T> &, const Optional<U> &) noexcept;
		template <typename T, typename U> bool operator > (const Optional<T> &, const Optional<U> &) noexcept;
		template <typename T, typename U> bool operator <=(const Optional<T> &, const Optional<U> &) noexcept;
		template <typename T, typename U> bool operator >=(const Optional<T> &, const Optional<U> &) noexcept;
		template <typename T, typename U> bool operator ==(const Optional<T> &, const Optional<U> &) noexcept;
		template <typename T, typename U> bool operator !=(const Optional<T> &, const Optional<U> &) noexcept;

		// nullptr relational operators
		template <typename T> bool operator ==(const Optional<T> &, std::nullptr_t) noexcept;
		template <typename T> bool operator ==(std::nullptr_t, const Optional<T> &) noexcept;
		template <typename T> bool operator !=(const Optional<T> &, std::nullptr_t) noexcept;
		template <typename T> bool operator !=(std::nullptr_t, const Optional<T> &) noexcept;
		
		// specialized algorithms
		template <typename T> void swap(Optional<T> &, Optional<T> &) noexcept;
	}
}

#	include "Optional.tpp"
#endif
