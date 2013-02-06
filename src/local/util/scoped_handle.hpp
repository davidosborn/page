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

#ifndef    page_local_util_scoped_handle_hpp
#   define page_local_util_scoped_handle_hpp

#	include <cstddef> // nullptr_t
#	include <functional> // function
#	include "NonCopyable.hpp"
#	include "Optional.hpp"

namespace page
{
	namespace util
	{
		/**
		 * Scoped handle wrapper with custom deleter.
		 *
		 * @note Inspired by discussions on the Boost mailing list.
		 * @see http://lists.boost.org/Archives/boost/2003/02/44741.php
		 * @see http://lists.boost.org/Archives/boost/2003/07/49827.php
		 *
		 * @note This class supports incomplete types.
		 * @note The deleter may be initialized to @c nullptr on construction,
		 *       in which case it will not be executed on destruction.
		 *
		 * @ingroup smart-pointer
		 */
		template <typename T> struct scoped_handle : NonCopyable
		{
			typedef T handle_type;
			typedef std::function<void (handle_type)> deleter_type;

			// constructors
			scoped_handle();
			explicit scoped_handle(handle_type);
			scoped_handle(handle_type, deleter_type);
			explicit scoped_handle(std::nullptr_t);
			scoped_handle(std::nullptr_t, deleter_type);

			// destructor
			~scoped_handle();

			// observers
			handle_type &operator  *() noexcept; const handle_type &operator  *() const noexcept;
			handle_type *operator ->() noexcept; const handle_type *operator ->() const noexcept;
			handle_type *get()         noexcept; const handle_type *get()         const noexcept;
			const deleter_type &get_deleter() const noexcept;
			explicit operator bool() const noexcept;

			// modifiers
			handle_type release() noexcept;
			void reset();
			void reset(handle_type);
			void reset(handle_type, deleter_type);
			void reset(std::nullptr_t);
			void reset(std::nullptr_t, deleter_type);
			void swap(scoped_handle &) noexcept;

			private:
			class DefaultDeleter;

			Optional<handle_type> handle;
			deleter_type deleter;
		};
		
		// factory functions
		template <typename T> scoped_handle<T> make_scoped_handle(T);
		template <typename T> scoped_handle<T> make_scoped_handle(T, typename scoped_handle<T>::deleter_type);

		// specialized algorithms
		template <typename T> void swap(scoped_handle<T> &, scoped_handle<T> &) noexcept;
	}
}

#	include "scoped_handle.tpp"
#endif
