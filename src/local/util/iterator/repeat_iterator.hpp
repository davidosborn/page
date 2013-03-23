/**
 * @section license
 *
 * Copyright (c) 2006-2013 David Osborn
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

#ifndef    page_local_util_iterator_repeat_iterator_hpp
#   define page_local_util_iterator_repeat_iterator_hpp

#	include <iterator> // random_access_iterator_tag
#	include <limits> // numeric_limits

#	include "iterator_facade.hpp"

namespace page
{
	namespace util
	{
		template <typename Value> struct repeat_iterator :
			iterator_facade<repeat_iterator<Value>,
				std::random_access_iterator_tag, const Value>
		{
			template <typename> friend class repeat_iterator;

			friend class iterator_facade<repeat_iterator<Value>,
				std::random_access_iterator_tag, const Value>;

			typedef iterator_facade<repeat_iterator<Value>,
				std::random_access_iterator_tag, const Value> Base;

			// HACK: GCC doesn't support using typename (bug #14258)
			typedef typename Base::reference reference;
			typedef typename Base::difference_type difference_type;

			// constructors
			repeat_iterator();
			explicit repeat_iterator(const Value &, difference_type = std::numeric_limits<difference_type>::max());
			template <typename Value2>
				repeat_iterator(const repeat_iterator<Value2> &);

			private:
			// dereference
			reference Dereference() const;

			// arithmetic
			void Increment();
			void Decrement();
			void Advance(difference_type);

			// distance
			difference_type Distance(const repeat_iterator &) const;

			// comparison
			bool Equal(const repeat_iterator &) const;
			bool Less(const repeat_iterator &) const;

			Value value;
			difference_type count;
		};

		// factory function template
		template <typename Value>
			inline repeat_iterator<Value> make_repeat_iterator(Value value)
			{ return repeat_iterator<Value>(value); }
		template <typename Value>
			inline repeat_iterator<Value> make_repeat_iterator(Value value,
				typename repeat_iterator<Value>::difference_type count)
			{ return repeat_iterator<Value>(value, count); }
	}
}

#	include "repeat_iterator.tpp"
#endif
