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

// function call iterator adapter

#ifndef    page_local_util_iterator_call_iterator_hpp
#   define page_local_util_iterator_call_iterator_hpp

#	include <functional> // result_of
#	include <iterator> // input_iterator_tag, iterator_traits

#	include <boost/optional.hpp>

#	include "iterator_adapter.hpp"

namespace page
{
	namespace util
	{
		template <typename Iterator, typename Function, typename Result =
			typename std::result_of<Function(
				typename std::iterator_traits<Iterator>::reference)>::type>
			struct call_iterator :
				iterator_adapter<call_iterator<Iterator, Function, Result>,
					Iterator, std::input_iterator_tag, Result,
					typename std::iterator_traits<Iterator>::difference_type>
		{
			friend class iterator_facade<
				call_iterator<Iterator, Function, Result>,
				std::input_iterator_tag, Result,
				typename std::iterator_traits<Iterator>::difference_type>;

			typedef iterator_adapter<call_iterator<Iterator, Function, Result>,
				Iterator, std::input_iterator_tag, Result,
				typename std::iterator_traits<Iterator>::difference_type> Base;

			// HACK: GCC doesn't support using typename (bug #14258)
			typedef typename Base::difference_type difference_type;
			typedef typename Base::reference reference;

			// constructors
			call_iterator();
			explicit call_iterator(const Iterator &, const Function &);

			private:
			// dereference
			reference Dereference() const;

			// arithmetic
			void Increment();

			Function func;
			typedef boost::optional<Result> Cache;
			mutable Cache cache;
		};
		template <typename Iterator, typename Function>
			struct call_iterator<Iterator, Function, void> :
				iterator_adapter<call_iterator<Iterator, Function, void>,
					Iterator, std::input_iterator_tag, void,
					typename std::iterator_traits<Iterator>::difference_type,
					void, void>
		{
			friend class iterator_facade<
				call_iterator<Iterator, Function, void>,
				std::input_iterator_tag, void,
				typename std::iterator_traits<Iterator>::difference_type,
				void, void>;

			typedef iterator_adapter<call_iterator<Iterator, Function, void>,
				Iterator, std::input_iterator_tag, void,
				typename std::iterator_traits<Iterator>::difference_type,
				void, void> Base;

			// HACK: GCC doesn't support using typename (bug #14258)
			typedef typename Base::difference_type difference_type;

			// constructors
			call_iterator();
			call_iterator(const Iterator &, const Function &);

			private:
			// dereference
			void Dereference() const;

			// arithmetic
			void Increment();

			Function func;
			mutable bool cache;
		};

		// factory function template
		template <typename Iterator, typename Function>
			inline call_iterator<Iterator, Function> make_call_iterator(Iterator iter, Function func)
			{ return call_iterator<Iterator, Function>(iter, func); }
	}
}

#	include "call_iterator.tpp"
#endif
