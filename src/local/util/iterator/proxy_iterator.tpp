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
		// proxy iterator
		// constructors
		template <typename Iterator, typename UnaryOperation, typename ConstUnaryOperation, typename Result, typename ConstResult>
			proxy_iterator<Iterator, UnaryOperation, ConstUnaryOperation, Result, ConstResult>::proxy_iterator() {}
		template <typename Iterator, typename UnaryOperation, typename ConstUnaryOperation, typename Result, typename ConstResult>
			proxy_iterator<Iterator, UnaryOperation, ConstUnaryOperation, Result, ConstResult>::proxy_iterator(Iterator iter, UnaryOperation op, ConstUnaryOperation cop) :
			Base(iter), op(op), cop(cop) {}
		template <typename Iterator, typename UnaryOperation, typename ConstUnaryOperation, typename Result, typename ConstResult>
			template <typename Iterator2>
			proxy_iterator<Iterator, UnaryOperation, ConstUnaryOperation, Result, ConstResult>::proxy_iterator(const proxy_iterator<Iterator2, UnaryOperation, ConstUnaryOperation, Result, ConstResult> &other) :
			Base(other.iter), op(other.op), cop(other.cop) {}

		// dereference
		template <typename Iterator, typename UnaryOperation, typename ConstUnaryOperation, typename Result, typename ConstResult>
			typename proxy_iterator<Iterator, UnaryOperation, ConstUnaryOperation, Result, ConstResult>::reference
			proxy_iterator<Iterator, UnaryOperation, ConstUnaryOperation, Result, ConstResult>::operator *() const
		{
			return Dereference();
		}
		template <typename Iterator, typename UnaryOperation, typename ConstUnaryOperation, typename Result, typename ConstResult>
			typename proxy_iterator<Iterator, UnaryOperation, ConstUnaryOperation, Result, ConstResult>::pointer
			proxy_iterator<Iterator, UnaryOperation, ConstUnaryOperation, Result, ConstResult>::operator ->() const
		{
			return pointer(Dereference());
		}
		template <typename Iterator, typename UnaryOperation, typename ConstUnaryOperation, typename Result, typename ConstResult>
			typename proxy_iterator<Iterator, UnaryOperation, ConstUnaryOperation, Result, ConstResult>::reference
			proxy_iterator<Iterator, UnaryOperation, ConstUnaryOperation, Result, ConstResult>::operator [](difference_type i) const
		{
			return (*this + i).Dereference();
		}

		// dereference
		template <typename Iterator, typename UnaryOperation, typename ConstUnaryOperation, typename Result, typename ConstResult>
			typename proxy_iterator<Iterator, UnaryOperation, ConstUnaryOperation, Result, ConstResult>::reference
			proxy_iterator<Iterator, UnaryOperation, ConstUnaryOperation, Result, ConstResult>::Dereference() const
		{
			return op(*this->iter);
		}

		// const proxy iterator
		// constructors
		template <typename Iterator, typename UnaryOperation, typename Result>
			const_proxy_iterator<Iterator, UnaryOperation, Result>::const_proxy_iterator() {}
		template <typename Iterator, typename UnaryOperation, typename Result>
			const_proxy_iterator<Iterator, UnaryOperation, Result>::const_proxy_iterator(Iterator iter, UnaryOperation op) :
			Base(iter), op(op) {}
		template <typename Iterator, typename UnaryOperation, typename Result>
			template <typename Iterator2>
			const_proxy_iterator<Iterator, UnaryOperation, Result>::const_proxy_iterator(const const_proxy_iterator<Iterator2, UnaryOperation, Result> &other) :
			Base(other.iter), op(other.op) {}
		template <typename Iterator, typename UnaryOperation, typename Result>
			template <typename NonConstUnaryOperation, typename NonConstResult>
			const_proxy_iterator<Iterator, UnaryOperation, Result>::const_proxy_iterator(const proxy_iterator<Iterator, NonConstUnaryOperation, UnaryOperation, NonConstResult, Result> &other) :
			Base(other.iter), op(other.cop) {}

		// dereference
		template <typename Iterator, typename UnaryOperation, typename Result>
			typename const_proxy_iterator<Iterator, UnaryOperation, Result>::reference
			const_proxy_iterator<Iterator, UnaryOperation, Result>::operator *() const
		{
			return Dereference();
		}
		template <typename Iterator, typename UnaryOperation, typename Result>
			typename const_proxy_iterator<Iterator, UnaryOperation, Result>::pointer
			const_proxy_iterator<Iterator, UnaryOperation, Result>::operator ->() const
		{
			return pointer(Dereference());
		}
		template <typename Iterator, typename UnaryOperation, typename Result>
			typename const_proxy_iterator<Iterator, UnaryOperation, Result>::reference
			const_proxy_iterator<Iterator, UnaryOperation, Result>::operator [](difference_type i) const
		{
			return (*this + i).Dereference();
		}

		// dereference
		template <typename Iterator, typename UnaryOperation, typename Result>
			typename const_proxy_iterator<Iterator, UnaryOperation, Result>::reference
			const_proxy_iterator<Iterator, UnaryOperation, Result>::Dereference() const
		{
			return op(*this->iter);
		}

		// reverse arithmetic
		template <typename Iterator, typename UnaryOperation, typename Result>
			const_proxy_iterator<Iterator, UnaryOperation, Result> operator +(
				typename const_proxy_iterator<Iterator, UnaryOperation, Result>::difference_type n,
				const const_proxy_iterator<Iterator, UnaryOperation, Result> &iter)
		{
			return iter + n;
		}
		template <typename Iterator, typename UnaryOperation, typename Result, typename ConstUnaryOperation, typename ConstResult>
			proxy_iterator<Iterator, UnaryOperation, Result, ConstUnaryOperation, ConstResult> operator +(
				typename proxy_iterator<Iterator, UnaryOperation, Result, ConstUnaryOperation, ConstResult>::difference_type n,
				const proxy_iterator<Iterator, UnaryOperation, Result, ConstUnaryOperation, ConstResult> &iter)
		{
			return iter + n;
		}
	}
}
