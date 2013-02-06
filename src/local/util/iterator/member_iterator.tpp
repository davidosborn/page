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

namespace page
{
	namespace util
	{
		// constructors
		template <typename Iterator, typename Value>
			member_iterator<Iterator, Value>::member_iterator(const Iterator &iter, Value member_of::*member) :
			Base(iter), member(member) {}
		template <typename Iterator, typename Value>
			template <typename Iterator2, typename Value2>
			member_iterator<Iterator, Value>::member_iterator(const member_iterator<Iterator2, Value2> &other) :
			Base(other.iter), member(other.member) {}

		// dereference
		template <typename Iterator, typename Value>
			typename member_iterator<Iterator, Value>::reference
			member_iterator<Iterator, Value>::Dereference() const
		{
			return (*this->iter).*member;
		}

		// factory function template
		template <typename Iterator, typename Value>
			member_iterator<Iterator, Value> make_member_iterator(Iterator iter, Value member_iterator<Iterator, Value>::member_of::*member)
		{
			return member_iterator<Iterator, Value>(iter, member);
		}
	}
}
