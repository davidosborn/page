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

#ifndef    page_local_util_iterator_proxy_iterator_hpp
#   define page_local_util_iterator_proxy_iterator_hpp

#	include <functional> // result_of
#	include <iterator> // iterator_traits

#	include "iterator_adapter.hpp"

namespace page
{
	namespace util
	{
		namespace detail
		{
			template <typename T> struct ProxyIteratorPointer
			{
				typedef T value_type;
				typedef const value_type *pointer;

				explicit ProxyIteratorPointer(const value_type &value) : value(value) {}
				pointer operator ->() const { return &value; }

				private:
				value_type value;
			};
		}

		template <typename Iterator, typename UnaryOperation, typename ConstUnaryOperation,
			typename Result = typename std::result_of<UnaryOperation(typename std::iterator_traits<Iterator>::reference)>::type,
			typename ConstResult = typename std::result_of<ConstUnaryOperation(typename std::iterator_traits<Iterator>::reference)>::type>
			struct proxy_iterator : iterator_adapter<
				proxy_iterator<Iterator, UnaryOperation, ConstUnaryOperation, Result, ConstResult>, Iterator,
				typename std::iterator_traits<Iterator>::iterator_category, Result,
				typename std::iterator_traits<Iterator>::difference_type,
				detail::ProxyIteratorPointer<Result>, Result>
		{
			template <typename, typename, typename, typename, typename> friend class proxy_iterator;
			template <typename, typename, typename> friend class const_proxy_iterator;

			typedef iterator_adapter<
				proxy_iterator<Iterator, UnaryOperation, ConstUnaryOperation, Result, ConstResult>, Iterator,
				typename std::iterator_traits<Iterator>::iterator_category, Result,
				typename std::iterator_traits<Iterator>::difference_type,
				detail::ProxyIteratorPointer<Result>, Result> Base;

			// HACK: GCC doesn't support using typename (bug #14258)
			typedef typename Base::difference_type difference_type;
			typedef typename Base::pointer pointer;
			typedef typename Base::reference reference;

			// constructors
			proxy_iterator();
			proxy_iterator(Iterator, UnaryOperation, ConstUnaryOperation);
			template <typename Iterator2>
				proxy_iterator(const proxy_iterator<Iterator2, UnaryOperation, ConstUnaryOperation, Result, ConstResult> &);

			// dereference
			reference operator *() const;
			pointer operator ->() const;
			reference operator [](difference_type) const;

			private:
			// dereference
			reference Dereference() const;

			UnaryOperation op;
			ConstUnaryOperation cop;
		};
		template <typename Iterator, typename UnaryOperation, typename Result =
			typename std::result_of<UnaryOperation(typename std::iterator_traits<Iterator>::reference)>::type>
			struct const_proxy_iterator : iterator_adapter<
				const_proxy_iterator<Iterator, UnaryOperation, Result>, Iterator,
				typename std::iterator_traits<Iterator>::iterator_category, Result,
				typename std::iterator_traits<Iterator>::difference_type,
				detail::ProxyIteratorPointer<Result>, Result>
		{
			template <typename, typename, typename> friend class const_proxy_iterator;

			typedef iterator_adapter<
				const_proxy_iterator<Iterator, UnaryOperation, Result>, Iterator,
				typename std::iterator_traits<Iterator>::iterator_category, Result,
				typename std::iterator_traits<Iterator>::difference_type,
				detail::ProxyIteratorPointer<Result>, Result> Base;

			// HACK: GCC doesn't support using typename (bug #14258)
			typedef typename Base::difference_type difference_type;
			typedef typename Base::pointer pointer;
			typedef typename Base::reference reference;

			// constructors
			const_proxy_iterator();
			const_proxy_iterator(Iterator, UnaryOperation);
			template <typename Iterator2>
				const_proxy_iterator(const const_proxy_iterator<Iterator2, UnaryOperation, Result> &);
			template <typename NonConstUnaryOperation, typename NonConstResult>
				const_proxy_iterator(const proxy_iterator<Iterator, NonConstUnaryOperation, UnaryOperation, NonConstResult, Result> &);

			// dereference
			reference operator *() const;
			pointer operator ->() const;
			reference operator [](difference_type) const;

			private:
			// dereference
			reference Dereference() const;

			UnaryOperation op;
		};

		// reverse arithmetic
		template <typename Iterator, typename UnaryOperation, typename Result>
			const_proxy_iterator<Iterator, UnaryOperation, Result> operator +(
				typename const_proxy_iterator<Iterator, UnaryOperation, Result>::difference_type,
				const const_proxy_iterator<Iterator, UnaryOperation, Result> &);
		template <typename Iterator, typename UnaryOperation, typename Result, typename ConstUnaryOperation, typename ConstResult>
			proxy_iterator<Iterator, UnaryOperation, Result, ConstUnaryOperation, ConstResult> operator +(
				typename proxy_iterator<Iterator, UnaryOperation, Result, ConstUnaryOperation, ConstResult>::difference_type,
				const proxy_iterator<Iterator, UnaryOperation, Result, ConstUnaryOperation, ConstResult> &);

		// factory function templates
		template <typename Iterator, typename UnaryOperation>
			inline const_proxy_iterator<Iterator, UnaryOperation> make_const_proxy_iterator(Iterator iter, UnaryOperation op)
			{ return const_proxy_iterator<Iterator, UnaryOperation>(iter, op); }
		template <typename Iterator, typename UnaryOperation, typename ConstUnaryOperation>
			inline proxy_iterator<Iterator, UnaryOperation, ConstUnaryOperation> make_proxy_iterator(Iterator iter, UnaryOperation op, ConstUnaryOperation cop)
			{ return proxy_iterator<Iterator, UnaryOperation, ConstUnaryOperation>(iter, op, cop); }
	}
}

#	include "proxy_iterator.tpp"
#endif
