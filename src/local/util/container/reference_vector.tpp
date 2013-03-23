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

#include <boost/iterator/transform_iterator.hpp>
#include <boost/range/adaptor/transformed.hpp>

#include "../functional/pointer.hpp" // address_of

namespace page
{
	namespace util
	{
		/*--------------------------+
		| constructors & destructor |
		+--------------------------*/

		template <typename T>
			reference_vector<T>::reference_vector() {}

		template <typename T>
			reference_vector<T>::reference_vector(size_type n, value_type value) :
				container(n, &value) {}

		template <typename T>
			template <typename InputIterator>
				reference_vector<T>::reference_vector(InputIterator first, InputIterator last) :
					container(
						boost::make_transform_iterator(first, address_of<typename InputIterator::value_type>()),
						boost::make_transform_iterator(last,  address_of<typename InputIterator::value_type>())) {}

		template <typename T>
			reference_vector<T>::reference_vector(std::initializer_list<typename container_type::value_type> il) :
				container(il)

				/**
				 * This is the code we would use if std::initializer_list supported
				 * reference types.
				 */
				/*container(boost::adaptors::transform(il, address_of<value_type>()))*/ {}

		/*----------+
		| iterators |
		+----------*/

		template <typename T>
			typename reference_vector<T>::iterator
				reference_vector<T>::begin()
		{
			return iterator(container.begin());
		}

		template <typename T>
			typename reference_vector<T>::const_iterator
				reference_vector<T>::begin() const
		{
			return const_iterator(container.begin());
		}

		template <typename T>
			typename reference_vector<T>::iterator
				reference_vector<T>::end()
		{
			return iterator(container.end());
		}

		template <typename T>
			typename reference_vector<T>::const_iterator
				reference_vector<T>::end() const
		{
			return const_iterator(container.end());
		}

		template <typename T>
			typename reference_vector<T>::const_iterator
				reference_vector<T>::cbegin() const
		{
			return const_iterator(container.cbegin());
		}

		template <typename T>
			typename reference_vector<T>::const_iterator
				reference_vector<T>::cend() const
		{
			return const_iterator(container.cend());
		}

		template <typename T>
			typename reference_vector<T>::reverse_iterator
				reference_vector<T>::rbegin()
		{
			return reverse_iterator(container.rbegin());
		}

		template <typename T>
			typename reference_vector<T>::const_reverse_iterator
				reference_vector<T>::rbegin() const
		{
			return const_reverse_iterator(container.rbegin());
		}

		template <typename T>
			typename reference_vector<T>::reverse_iterator
				reference_vector<T>::rend()
		{
			return reverse_iterator(container.rend());
		}

		template <typename T>
			typename reference_vector<T>::const_reverse_iterator
				reference_vector<T>::rend() const
		{
			return const_reverse_iterator(container.rend());
		}

		template <typename T>
			typename reference_vector<T>::const_reverse_iterator
				reference_vector<T>::crbegin() const
		{
			return const_reverse_iterator(container.crbegin());
		}

		template <typename T>
			typename reference_vector<T>::const_reverse_iterator
				reference_vector<T>::crend() const
		{
			return const_reverse_iterator(container.crend());
		}

		/*---------+
		| capacity |
		+---------*/

		template <typename T>
			typename reference_vector<T>::size_type
				reference_vector<T>::size() const
		{
			return container.size();
		}

		template <typename T>
			typename reference_vector<T>::size_type
				reference_vector<T>::max_size() const
		{
			return container.max_size();
		}

		template <typename T>
			bool reference_vector<T>::empty() const
		{
			return container.empty();
		}

		template <typename T>
			void reference_vector<T>::resize(size_type n, value_type value)
		{
			container.resize(n, &value);
		}

		template <typename T>
			typename reference_vector<T>::size_type
				reference_vector<T>::capacity() const
		{
			return container.capacity();
		}

		template <typename T>
			void reference_vector<T>::reserve(size_type n)
		{
			container.reserve(n);
		}

		template <typename T>
			void reference_vector<T>::shrink_to_fit()
		{
			container.shrink_to_fit();
		}

		/*---------------+
		| element access |
		+---------------*/

		template <typename T>
			typename reference_vector<T>::reference
				reference_vector<T>::operator [](size_type i)
		{
			return *container[i];
		}

		template <typename T>
			typename reference_vector<T>::const_reference
				reference_vector<T>::operator [](size_type i) const
		{
			return *container[i];
		}

		template <typename T>
			typename reference_vector<T>::reference
				reference_vector<T>::at(size_type i)
		{
			return *container.at(i);
		}

		template <typename T>
			typename reference_vector<T>::const_reference
				reference_vector<T>::at(size_type i) const
		{
			return *container.at(i);
		}

		template <typename T>
			typename reference_vector<T>::reference
				reference_vector<T>::front()
		{
			return *container.front();
		}

		template <typename T>
			typename reference_vector<T>::const_reference
				reference_vector<T>::front() const
		{
			return *container.front();
		}

		template <typename T>
			typename reference_vector<T>::reference
				reference_vector<T>::back()
		{
			return *container.back();
		}

		template <typename T>
			typename reference_vector<T>::const_reference
				reference_vector<T>::back() const
		{
			return *container.back();
		}

		/*----------+
		| modifiers |
		+----------*/

		template <typename T>
			void reference_vector<T>::swap(reference_vector &other)
		{
			return container.swap(other.container);
		}

		template <typename T>
			void reference_vector<T>::push_back(value_type value)
		{
			container.push_back(&value);
		}

		template <typename T>
			void reference_vector<T>::pop()
		{
			container.pop();
		}

		template <typename T>
			typename reference_vector<T>::iterator
				reference_vector<T>::insert(const_iterator iter, value_type value)
		{
			return iterator(container.insert(iter.base(), &value));
		}

		template <typename T>
			typename reference_vector<T>::iterator
				reference_vector<T>::insert(const_iterator iter, size_type n, value_type value)
		{
			return iterator(container.insert(iter.base(), n, &value));
		}

		template <typename T>
			template <typename InputIterator>
				typename reference_vector<T>::iterator
					reference_vector<T>::insert(const_iterator iter, InputIterator first, InputIterator last)
		{
			return iterator(container.insert(iter.base(),
				boost::make_transform_iterator(first, address_of<typename InputIterator::value_type>()),
				boost::make_transform_iterator(last,  address_of<typename InputIterator::value_type>())));
		}

		template <typename T>
			typename reference_vector<T>::iterator
				reference_vector<T>::insert(const_iterator iter, std::initializer_list<typename container_type::value_type> il)
		{
			return iterator(container.insert(iter.base(), il));

			/**
			 * This is the code we would use if std::initializer_list supported
			 * reference types.
			 */
			/*return iterator(container.insert(iter.base(),
				boost::adaptors::transform(il, address_of<value_type>())));*/
		}

		template <typename T>
			typename reference_vector<T>::iterator
				reference_vector<T>::erase(const_iterator iter)
		{
			return iterator(container.erase(iter.base()));
		}

		template <typename T>
			typename reference_vector<T>::iterator
				reference_vector<T>::erase(const_iterator first, const_iterator last)
		{
			return iterator(container.erase(first.base(), last.base()));
		}

		template <typename T>
			void reference_vector<T>::clear()
		{
			container.clear();
		}

		template <typename T>
			template <typename InputIterator>
				void reference_vector<T>::assign(InputIterator first, InputIterator last)
		{
			container.assign(
				boost::make_transform_iterator(first, address_of<typename InputIterator::value_type>()),
				boost::make_transform_iterator(last,  address_of<typename InputIterator::value_type>()));
		}

		template <typename T>
			void reference_vector<T>::assign(std::initializer_list<typename container_type::value_type> il)
		{
			container.assign(il);

			/**
			 * This is the code we would use if std::initializer_list supported
			 * reference types.
			 */
			//container.assign(boost::adaptors::transform(il, address_of<value_type>()));
		}

		template <typename T>
			void reference_vector<T>::assign(size_type n, value_type value)
		{
			container.assign(n, &value);
		}
	}
}
