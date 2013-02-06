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
 *
 * @file
 *
 * Utilities for declaring classes.
 */

#ifndef    page_local_util_class_hpp
#   define page_local_util_class_hpp

#	include <utility> // forward

	/**
	 * Define the copy-constructor and copy-assignment-operator of the specified
	 * class.  The value should be either "default" or "delete".
	 */
#	define DEFINE_COPY(CLASS, VALUE) \
		CLASS(const CLASS &)             = VALUE; \
		CLASS &operator =(const CLASS &) = VALUE;

	/**
	 * Define the move-constructor and move-assignment-operator of the specified
	 * class.  The value should be either "default" or "delete".
	 */
#	define DEFINE_MOVE(CLASS, VALUE) \
		CLASS(CLASS &&)             = VALUE; \
		CLASS &operator =(CLASS &&) = VALUE;

	/**
	 * Make the specified class "uncopyable" (ISO/IEC n3242 8.4.3.3).  The class
	 * will not be copyable, but it will be movable.  This behaviour is
	 * inherited, as long as the compiler is able to implicitly generate the
	 * move-constructor and move-assignment-operator of the derived class (in
	 * particular, it can't in the case of an user-defined destructor).
	 */
#	define MAKE_UNCOPYABLE(CLASS) \
		public: \
		DEFINE_COPY(CLASS, delete) \
		DEFINE_MOVE(CLASS, default)

	/**
	 * Cause the specified class to inherit the constructors of one of its base
	 * classes.
	 *
	 * @see ISO/IEC n1898, Forwarding and Inherited Constructors
	 */
#	define INHERIT_CONSTRUCTORS(CLASS, BASE) \
		template <typename... Args> \
			explicit CLASS(Args &&... args) : \
				BASE(std::forward<Args>(args)...) {}

namespace page
{
	namespace util
	{
		/**
		 * An intermediate class-template which inherits from the base classes
		 * specified as template arguments.
		 */
		template <typename... Bases>
			class PublicVirtualInheritor :
				public virtual Bases... {};
	}
}

#endif
