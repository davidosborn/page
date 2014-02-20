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

#include <utility> // forward

namespace page { namespace util
{
	/*-------------+
	| constructors |
	+-------------*/

	template <typename Signature> template <typename... Args>
		copyable_signal<Signature>::copyable_signal(Args &&... args) :
			boost::signal<Signature>(std::forward<Args>(args)...) {}

	/*--------------------+
	| copy/move semantics |
	+--------------------*/

	template <typename Signature>
		copyable_signal<Signature>::copyable_signal(const copyable_signal &other) {}

	template <typename Signature>
		copyable_signal<Signature> &
		copyable_signal<Signature>::operator =(const copyable_signal &other) {}

	template <typename Signature>
		copyable_signal<Signature>::copyable_signal(copyable_signal &&other)
	{
		/**
		 * Exploiting undocumented behaviour to copy the signal's connections.
		 * @sa http://thread.gmane.org/gmane.comp.lib.boost.user/75108
		 */
		this->connect(other);
	}

	template <typename Signature>
		copyable_signal<Signature> &
		copyable_signal<Signature>::operator =(copyable_signal &&other)
	{
		/**
		 * Exploiting undocumented behaviour to copy the signal's connections.
		 * @sa http://thread.gmane.org/gmane.comp.lib.boost.user/75108
		 */
		this->connect(other);
	}
}}
