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

#ifndef    page_local_util_copyable_signal_hpp
#   define page_local_util_copyable_signal_hpp

#	include <boost/signal.hpp>

#	include "class/special_member_functions.hpp" // DEFINE_MOVE

namespace page { namespace util
{
	/**
	 * A wrapper around @c boost::signal to make it copyable.
	 *
	 * The connections are not copied when the signal is copied.
	 */
	template <typename Signature>
		class copyable_signal : public boost::signal<Signature>
	{
		/*-------------+
		| constructors |
		+-------------*/

		public:
		template <typename... Args>
			explicit copyable_signal(Args &&...);

		/*--------------------+
		| copy/move semantics |
		+--------------------*/

		/**
		 * Copies the signal.  The connections are not copied.
		 */
		copyable_signal(const copyable_signal &);

		/**
		 * Copies the signal.  The connections are not copied.
		 */
		copyable_signal &operator =(const copyable_signal &);

		/**
		 * Moves the signal.  The connections are retained.
		 */
		copyable_signal(copyable_signal &&);

		/**
		 * Moves the signal.  The connections are retained.
		 */
		copyable_signal &operator =(copyable_signal &&);
	};
}}

#	include "copyable_signal.tpp"
#endif
