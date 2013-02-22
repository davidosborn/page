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

#ifndef    page_local_util_class_Cloneable_hpp
#   define page_local_util_class_Cloneable_hpp

#	include "../raii/copy_ptr.hpp"
#	include "copy_move.hpp" // DEFINE_{COPY,MOVE}

namespace page
{
	namespace util
	{
		/**
		 * Makes the derived class cloneable, which means it has the @c Clone
		 * and @c Copy member functions declared here.
		 */
		template <typename Derived>
			class Cloneable
		{
			public:
			/**
			 * It is necessary to explicitly declare the default constructor
			 * because it will not be provided by default.
			 */
			Cloneable() = default;

			/**
			 * It is necessary to explicitly declare the copy constructor and
			 * assignment operator because they will not be provided by default.
			 */
			DEFINE_COPY(Cloneable, default)

			/**
			 * It is necessary to explicitly declare the move constructor and
			 * assignment operator because they will not be provided by default,
			 * since the class has a user-declared destructor
			 * (ISO/IEC N3242 12.8.10).
			 */
			DEFINE_MOVE(Cloneable, default)

			/**
			 * It is necessary to declare the destructor to be virtual to ensure
			 * that the destructor of the derived class will be called.
			 *
			 * @note Under GCC 4.7.1, the default destructor has a noexcept
			 *       exception specification.
			 *
			 * @note A loose exception specification is used here to permit
			 *       derived classes to also have a loose exception
			 *       specification.
			 */
			virtual ~Cloneable();

			/**
			 * @return A new copy of the object.
			 */
			virtual Derived *Clone() const = 0;

			/**
			 * @return A copyable smart-pointer, which uses @c Clone to copy the
			 *         object.
			 */
			copy_ptr<Derived> Copy() const
			{
				// NOTE: affected by GCC bug #35637
				return copy_ptr<Derived>(Clone(), &Cloneable::Clone);
			}
		};

		template <typename Derived>
			Cloneable<Derived>::~Cloneable() = default;
	}
}

#endif
