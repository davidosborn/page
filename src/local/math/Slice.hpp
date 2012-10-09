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

#ifndef    page_local_math_Slice_hpp
#   define page_local_math_Slice_hpp

#	include <cstddef> // size_t
#	include <iosfwd> // [io]stream
#	include <iterator> // iterator_traits, reverse_iterator
#	include "ArithmeticConversion.hpp"
#	include "Vector.hpp"

namespace page
{
	namespace math
	{
		/**
		 * Matrix slice proxy.
		 *
		 * Partially emulates a vector.
		 */
		template <unsigned n, typename Iterator> struct BasicSlice
		{
			template <unsigned, typename> friend class BasicSlice;

			// container traits
			typedef typename std::iterator_traits<Iterator>::value_type value_type;
			// HACK: the constness of Slice depends on Iterator, so the
			// reference and iterator types are uniformly qualified
			typedef typename std::iterator_traits<Iterator>::reference reference;
			typedef typename std::iterator_traits<Iterator>::reference const_reference;
			typedef Iterator iterator;
			typedef Iterator const_iterator;
			typedef std::reverse_iterator<iterator> reverse_iterator;
			typedef std::reverse_iterator<const_iterator> const_reverse_iterator;
			typedef typename std::iterator_traits<Iterator>::difference_type difference_type;
			typedef std::size_t size_type;

			protected:
			// constructors
			explicit BasicSlice(const Iterator &);
			template <typename Iterator2> BasicSlice(const BasicSlice<n, Iterator2> &);
			
			// assignment
			const BasicSlice &operator =(const BasicSlice &) const;
			template <typename Iterator2> const BasicSlice &operator =(const BasicSlice<n, Iterator2> &) const;
			const BasicSlice &operator =(const value_type &) const;
			const BasicSlice &operator =(const Vector<n, value_type> &) const;

			public:
			// conversion
			operator Vector<n, value_type>() const;

			// iterators
			iterator begin() const;
			iterator end() const;

			// reverse iterators
			reverse_iterator rbegin() const;
			reverse_iterator rend() const;

			// element access
			reference operator [](size_type) const;

			private:
			const Iterator iter;
		};
		template <unsigned n, typename Iterator> struct Slice : BasicSlice<n, Iterator>
		{
			typedef BasicSlice<n, Iterator> Base;

			// HACK: GCC doesn't support using typename (bug #14258)
			typedef typename Base::value_type value_type;

			// constructors
			explicit Slice(const Iterator &);
			template <typename Iterator2> Slice(const Slice<n, Iterator2> &);
			
			// assignment
			template <typename Iterator2> const Slice &operator =(const Slice<n, Iterator2> &) const;
			const Slice &operator =(const value_type &) const;
			const Slice &operator =(const Vector<n, value_type> &) const;
		};
		template <typename Iterator> struct Slice<2, Iterator> : BasicSlice<2, Iterator>
		{
			typedef BasicSlice<2, Iterator> Base;

			// HACK: GCC doesn't support using typename (bug #14258)
			typedef typename Base::value_type value_type;
			typedef typename Base::reference reference;

			// constructors
			explicit Slice(const Iterator &);
			Slice(const Slice &);
			template <typename Iterator2> Slice(const Slice<2, Iterator2> &);
			
			// assignment
			const Slice &operator =(const Slice &) const;
			template <typename Iterator2> const Slice &operator =(const Slice<2, Iterator2> &) const;
			const Slice &operator =(const value_type &) const;
			const Slice &operator =(const Vector<2, value_type> &) const;

			reference x, y;

			using Base::begin;
			using Base::end;
		};
		template <typename Iterator> struct Slice<3, Iterator> : BasicSlice<3, Iterator>
		{
			typedef BasicSlice<3, Iterator> Base;

			// HACK: GCC doesn't support using typename (bug #14258)
			typedef typename Base::value_type value_type;
			typedef typename Base::reference reference;

			// constructors
			explicit Slice(const Iterator &);
			Slice(const Slice &);
			template <typename Iterator2> Slice(const Slice<3, Iterator2> &);
			
			// assignment
			const Slice &operator =(const Slice &) const;
			template <typename Iterator2> const Slice &operator =(const Slice<3, Iterator2> &) const;
			const Slice &operator =(const value_type &) const;
			const Slice &operator =(const Vector<3, value_type> &) const;

			reference x, y, z;

			using Base::begin;
			using Base::end;
		};
		template <typename Iterator> struct Slice<4, Iterator> : BasicSlice<4, Iterator>
		{
			typedef BasicSlice<4, Iterator> Base;

			// HACK: GCC doesn't support using typename (bug #14258)
			typedef typename Base::value_type value_type;
			typedef typename Base::reference reference;

			// constructors
			explicit Slice(const Iterator &);
			Slice(const Slice &);
			template <typename Iterator2> Slice(const Slice<4, Iterator2> &);
			
			// assignment
			const Slice &operator =(const Slice &) const;
			template <typename Iterator2> const Slice &operator =(const Slice<4, Iterator2> &) const;
			const Slice &operator =(const value_type &) const;
			const Slice &operator =(const Vector<4, value_type> &) const;

			reference x, y, z, w;

			using Base::begin;
			using Base::end;
		};

		// unary operators
		template <unsigned n, typename Iterator> const Slice<n, Iterator> &operator ++(const Slice<n, Iterator> &);
		template <unsigned n, typename Iterator> const Slice<n, Iterator> &operator --(const Slice<n, Iterator> &);
		template <unsigned n, typename Iterator> Vector<n, typename Slice<n, Iterator>::value_type> operator ++(const Slice<n, Iterator> &, int);
		template <unsigned n, typename Iterator> Vector<n, typename Slice<n, Iterator>::value_type> operator --(const Slice<n, Iterator> &, int);
		template <unsigned n, typename Iterator> Vector<n, typename Slice<n, Iterator>::value_type> operator +(const Slice<n, Iterator> &);
		template <unsigned n, typename Iterator> Vector<n, typename Slice<n, Iterator>::value_type> operator -(const Slice<n, Iterator> &);

		// assignment operators
		template <unsigned n, typename Iterator, typename Iterator2> const Slice<n, Iterator> &operator +=(const Slice<n, Iterator> &, const Slice<n, Iterator2> &);
		template <unsigned n, typename Iterator, typename Iterator2> const Slice<n, Iterator> &operator -=(const Slice<n, Iterator> &, const Slice<n, Iterator2> &);
		template <unsigned n, typename Iterator, typename Iterator2> const Slice<n, Iterator> &operator *=(const Slice<n, Iterator> &, const Slice<n, Iterator2> &);
		template <unsigned n, typename Iterator, typename Iterator2> const Slice<n, Iterator> &operator /=(const Slice<n, Iterator> &, const Slice<n, Iterator2> &);
		template <unsigned n, typename Iterator, typename Iterator2> const Slice<n, Iterator> &operator %=(const Slice<n, Iterator> &, const Slice<n, Iterator2> &);

		// vector assignment operators
		template <unsigned n, typename Iterator, typename T> const Slice<n, Iterator> &operator +=(const Slice<n, Iterator> &, const Vector<n, T> &);
		template <unsigned n, typename Iterator, typename T> const Slice<n, Iterator> &operator -=(const Slice<n, Iterator> &, const Vector<n, T> &);
		template <unsigned n, typename Iterator, typename T> const Slice<n, Iterator> &operator *=(const Slice<n, Iterator> &, const Vector<n, T> &);
		template <unsigned n, typename Iterator, typename T> const Slice<n, Iterator> &operator /=(const Slice<n, Iterator> &, const Vector<n, T> &);
		template <unsigned n, typename Iterator, typename T> const Slice<n, Iterator> &operator %=(const Slice<n, Iterator> &, const Vector<n, T> &);

		// scalar assignment operators
		template <unsigned n, typename Iterator, typename T> const Slice<n, Iterator> &operator +=(const Slice<n, Iterator> &, T);
		template <unsigned n, typename Iterator, typename T> const Slice<n, Iterator> &operator -=(const Slice<n, Iterator> &, T);
		template <unsigned n, typename Iterator, typename T> const Slice<n, Iterator> &operator *=(const Slice<n, Iterator> &, T);
		template <unsigned n, typename Iterator, typename T> const Slice<n, Iterator> &operator /=(const Slice<n, Iterator> &, T);
		template <unsigned n, typename Iterator, typename T> const Slice<n, Iterator> &operator %=(const Slice<n, Iterator> &, T);

		// stream insertion/extraction
		template <unsigned n, typename Iterator> std::ostream &operator <<(std::ostream &, const Slice<n, Iterator> &);
		template <unsigned n, typename Iterator> std::istream &operator >>(std::istream &, const Slice<n, Iterator> &);
	}
}

#	include "Slice.tpp"
#endif
