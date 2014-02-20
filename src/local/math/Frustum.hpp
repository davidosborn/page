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

#ifndef    page_math_Frustum_hpp
#   define page_math_Frustum_hpp

#	include <cstddef> // ptrdiff_t, size_t
#	include <iosfwd> // basic_[io]stream
#	include <iterator> // reverse_iterator

#	include "fwd.hpp" // Frustum, Matrix
#	include "Plane.hpp"

namespace page { namespace math
{
	/**
	 * General viewing frustum.
	 */
	template <typename T> struct Frustum
	{
		typedef math::Plane<3, T> Plane;

		// container traits
		typedef Plane value_type;
		typedef value_type &reference;
		typedef const value_type &const_reference;
		typedef value_type *iterator;
		typedef const value_type *const_iterator;
		typedef std::reverse_iterator<iterator> reverse_iterator;
		typedef std::reverse_iterator<const_iterator> const_reverse_iterator;
		typedef std::ptrdiff_t difference_type;
		typedef std::size_t size_type;

		// constructors
		explicit Frustum(const Plane & = Plane());
		Frustum(const Plane &, const Plane &, const Plane &, const Plane &, const Plane &, const Plane &);
		Frustum(const Frustum &);
		template <typename U> explicit Frustum(const Frustum<U> &);
		explicit Frustum(const Matrix<4, 4, T> &);

		// assignment
		Frustum &operator =(const Plane &);
		Frustum &operator =(const Frustum &);

		// conversion
		template <typename U> operator Frustum<U>() const;

		// iterators
		iterator begin();
		const_iterator begin() const;
		iterator end();
		const_iterator end() const;

		// reverse iterators
		reverse_iterator rbegin();
		const_reverse_iterator rbegin() const;
		reverse_iterator rend();
		const_reverse_iterator rend() const;

		// element access
		reference operator [](size_type);
		const_reference operator [](size_type) const;

		Plane &left, &right, &bottom, &top, &near, &far;

		private:
		Plane _data[6];
	};

	// stream insertion/extraction
	template <typename Char, typename CharTraits, typename T> std::basic_ostream<Char, CharTraits> &operator <<(std::basic_ostream<Char, CharTraits> &, const Frustum<T> &);
	template <typename Char, typename CharTraits, typename T> std::basic_istream<Char, CharTraits> &operator >>(std::basic_istream<Char, CharTraits> &, Frustum<T> &);

	// standard-library compatibility
	template <typename T> void swap(Frustum<T> &, Frustum<T> &);
}}

#	include "Frustum.tpp"
#endif
