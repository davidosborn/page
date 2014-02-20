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

#ifndef    page_local_util_raii_factory_ptr_hpp
#   define page_local_util_raii_factory_ptr_hpp

#	include <cstddef> // nullptr_t
#	include <functional> // function
#	include <memory> // pointer_traits

namespace page { namespace util
{
	/**
	 * A smart-pointer facade for nullary factory-functions.
	 */
	template <typename T>
		class factory_ptr
	{
		template <typename>
			friend class factory_ptr;

		/*-------+
		| traits |
		+-------*/

		public:
		/**
		 * The type of a pointer to the product, returned by the factory
		 * function.
		 */
		using pointer = T;

		/**
		 * The type of the product.
		 */
		using element_type = typename std::pointer_traits<T>::element_type;

		/**
		 * The type of the factory function.
		 */
		using factory_function = std::function<pointer()>;

		/*-------------+
		| constructors |
		+-------------*/

		factory_ptr() noexcept = default;

		factory_ptr(std::nullptr_t) noexcept;

		protected:
		explicit factory_ptr(const factory_function &);

		public:
		template <typename U>
			factory_ptr(const factory_ptr<U> &);

		/*----------+
		| observers |
		+----------*/

		/**
		 * @return @c *lock().
		 */
		element_type &operator *() const;

		/**
		 * @return @c lock().
		 */
		pointer operator ->() const;

		/**
		 * @return lock().
		 */
		pointer get() const;

		/**
		 * @return f().
		 */
		pointer lock() const;

		/**
		 * @return f.
		 */
		explicit operator bool() const noexcept;

		/*-------------+
		| data members |
		+-------------*/

		private:
		/**
		 * The factory function.
		 */
		factory_function f;
	};
}}

#	include "factory_ptr.tpp"
#endif
