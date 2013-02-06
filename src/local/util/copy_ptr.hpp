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

#ifndef    page_local_util_copy_ptr_hpp
#   define page_local_util_copy_ptr_hpp

#	include <cstddef> // nullptr_t
#	include <functional> // function
#	include <iosfwd> // ostream
#	include <memory> // {auto,unique}_ptr

namespace page
{
	namespace util
	{
		/**
		 * A copy pointer with a custom cloner/deleter.
		 *
		 * @note This class supports incomplete types.
		 * @note The cloner and deleter may be initialized to @c nullptr on
		 *       construction.  In this case, the copy constructor will produce
		 *       null pointers and the deleter will not be executed.
		 *
		 * @ingroup smart-pointer
		 */
		template <typename T> struct copy_ptr
		{
			template <typename> friend class copy_ptr;

			typedef T *pointer;
			typedef T element_type;
			typedef std::function<T *(T *)> cloner_type;
			typedef std::function<T *(const T *)> const_cloner_type;
			typedef std::function<void (T *)> deleter_type;
			typedef std::function<void (const T *)> const_deleter_type;

			// constructors
			copy_ptr();
			explicit copy_ptr(pointer);
			copy_ptr(pointer, cloner_type);
			copy_ptr(pointer, cloner_type, deleter_type);
			copy_ptr(std::nullptr_t);
			copy_ptr(std::nullptr_t, cloner_type);
			copy_ptr(std::nullptr_t, cloner_type, deleter_type);
			copy_ptr(const copy_ptr &);
			template <typename U> copy_ptr(const copy_ptr<U> &);
			template <typename U> copy_ptr(std::auto_ptr<U> &&);
			template <typename U> copy_ptr(std::auto_ptr<U> &&, cloner_type);
			template <typename U, typename D> copy_ptr(std::unique_ptr<U, D> &&);
			template <typename U, typename D> copy_ptr(std::unique_ptr<U, D> &&, cloner_type);

			// destructor
			~copy_ptr();

			// assignment
			copy_ptr &operator =(copy_ptr);
			template <typename U> copy_ptr &operator =(copy_ptr<U>);
			template <typename U> copy_ptr &operator =(std::auto_ptr<U> &&);
			template <typename U, typename D> copy_ptr &operator =(std::unique_ptr<U, D> &&);

			// observers
			T &operator *() const noexcept;
			pointer operator ->() const noexcept;
			pointer get() const noexcept;
			const cloner_type &get_cloner() const noexcept;
			/**
			 * @note This function silently casts the cloner to
			 *       @c const_cloner_type, even if the original cloner was
			 *       non-const.
			 */
			const_cloner_type get_const_cloner() const;
			const deleter_type &get_deleter() const noexcept;
			/**
			 * @note This function silently casts the deleter to
			 *       @c const_deleter_type, even if the original deleter was
			 *       non-const.
			 */
			const_deleter_type get_const_deleter() const;
			explicit operator bool() const noexcept;

			// modifiers
			pointer release() noexcept;
			void reset();
			template <typename U> void reset(U *);
			template <typename U, typename C> void reset(U *, C);
			template <typename U, typename C, typename D> void reset(U *, C, D);
			void reset(std::nullptr_t);
			template <typename C> void reset(std::nullptr_t, C);
			template <typename C, typename D> void reset(std::nullptr_t, C, D);
			void swap(copy_ptr &) noexcept;

			private:
			// default cloner/deleter
			class DefaultCloner;
			class DefaultDeleter;

			// const cloner/deleter adapters
			class ConstClonerAdapter;
			class ConstDeleterAdapter;

			// implicit cloner/deleter conversion adapters
			template <typename> class ClonerConversionAdapter;
			template <typename> class DeleterConversionAdapter;

			pointer      p;
			cloner_type  cloner;
			deleter_type deleter;
		};

		// relational operators
		template <typename T, typename U> bool operator < (const copy_ptr<T> &, const copy_ptr<U> &) noexcept;
		template <typename T, typename U> bool operator > (const copy_ptr<T> &, const copy_ptr<U> &) noexcept;
		template <typename T, typename U> bool operator <=(const copy_ptr<T> &, const copy_ptr<U> &) noexcept;
		template <typename T, typename U> bool operator >=(const copy_ptr<T> &, const copy_ptr<U> &) noexcept;
		template <typename T, typename U> bool operator ==(const copy_ptr<T> &, const copy_ptr<U> &) noexcept;
		template <typename T, typename U> bool operator !=(const copy_ptr<T> &, const copy_ptr<U> &) noexcept;

		// nullptr relational operators
		template <typename T> bool operator ==(const copy_ptr<T> &, std::nullptr_t) noexcept;
		template <typename T> bool operator ==(std::nullptr_t, const copy_ptr<T> &) noexcept;
		template <typename T> bool operator !=(const copy_ptr<T> &, std::nullptr_t) noexcept;
		template <typename T> bool operator !=(std::nullptr_t, const copy_ptr<T> &) noexcept;

		// casts
		template <typename T, typename U> copy_ptr<T>      static_pointer_cast(const copy_ptr<U> &) noexcept;
		template <typename T, typename U> copy_ptr<T>     dynamic_pointer_cast(const copy_ptr<U> &) noexcept;
		template <typename T, typename U> copy_ptr<T> reinterpret_pointer_cast(const copy_ptr<U> &) noexcept;
		template <typename T, typename U> copy_ptr<T>       const_pointer_cast(const copy_ptr<U> &) noexcept;

		// stream insertion
		template <typename T, typename Char, typename CharTraits>
			std::basic_ostream<Char, CharTraits> &operator <<(std::basic_ostream<Char, CharTraits> &, const copy_ptr<T> &) noexcept;

		// specialized algorithms
		template <typename T> void swap(copy_ptr<T> &, copy_ptr<T> &) noexcept;
	}
}

#	include "copy_ptr.tpp"
#endif
