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

#ifndef    page_local_util_container_reference_vector_hpp
#   define page_local_util_container_reference_vector_hpp

#	include <vector>

#	include <boost/iterator/indirect_iterator.hpp>

namespace page
{
	namespace util
	{
		/**
		 * A container that is like @c std::vector, except that it stores
		 * references to elements rather than the elements themselves.
		 *
		 * @note Because this container stores references instead of values,
		 *       the standard emplace functions are not provided.
		 *
		 * @note The specification for @c std::initializer_list doesn't permit
		 *       reference types, so we have to use pointers instead.
		 */
		template <typename T>
			class reference_vector
		{
			/*------+
			| types |
			+------*/

			private:
			typedef std::vector<T *> container_type;

			public:
			typedef T &value_type;
			typedef T &reference;
			typedef const T &const_reference;
			typedef boost::indirect_iterator<typename container_type::iterator> iterator;
			typedef boost::indirect_iterator<typename container_type::const_iterator> const_iterator;
			typedef boost::indirect_iterator<typename container_type::reverse_iterator> reverse_iterator;
			typedef boost::indirect_iterator<typename container_type::const_reverse_iterator> const_reverse_iterator;
			typedef typename container_type::difference_type difference_type;
			typedef typename container_type::size_type size_type;

			/*-------------+
			| constructors |
			+-------------*/

			public:
			reference_vector();
			reference_vector(size_type, value_type);
			template <typename InputIterator>
				reference_vector(InputIterator, InputIterator);
			reference_vector(std::initializer_list<typename container_type::value_type>);

			/*----------+
			| iterators |
			+----------*/

			public:
			iterator       begin();
			const_iterator begin()  const;
			iterator       end();
			const_iterator end()    const;
			const_iterator cbegin() const;
			const_iterator cend()   const;

			reverse_iterator       rbegin();
			const_reverse_iterator rbegin()  const;
			reverse_iterator       rend();
			const_reverse_iterator rend()    const;
			const_reverse_iterator crbegin() const;
			const_reverse_iterator crend()   const;

			/*---------+
			| capacity |
			+---------*/

			size_type size() const;
			size_type max_size() const;
			bool empty() const;

			void resize(size_type, value_type);
			size_type capacity() const;
			void reserve(size_type);
			void shrink_to_fit();

			/*---------------+
			| element access |
			+---------------*/

			reference       operator [](size_type);
			const_reference operator [](size_type) const;
			reference       at(size_type);
			const_reference at(size_type) const;
			reference       front();
			const_reference front() const;
			reference       back();
			const_reference back() const;

			/*----------+
			| modifiers |
			+----------*/

			public:
			void swap(reference_vector &);

			void push_back(value_type);
			void pop();

			iterator insert(const_iterator, value_type);
			iterator insert(const_iterator, size_type, value_type);
			template <typename Iterator>
				iterator insert(const_iterator, Iterator, Iterator);
			iterator insert(const_iterator, std::initializer_list<typename container_type::value_type>);

			iterator erase(const_iterator);
			iterator erase(const_iterator, const_iterator);

			void clear();

			template <typename Iterator>
				void assign(Iterator, Iterator);
			void assign(std::initializer_list<typename container_type::value_type>);
			void assign(size_type, value_type);

			/*-------------+
			| data members |
			+-------------*/

			private:
			container_type container;
		};
	}
}

#	include "reference_vector.tpp"
#endif
