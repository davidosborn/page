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

#ifndef    page_math_Plane_hpp
#   define page_math_Plane_hpp

#	include <iosfwd> // basic_[io]stream

#	include "fwd.hpp" // Plane
#	include "Vector.hpp"

namespace page { namespace math
{
	template <unsigned n, typename T> struct Plane
	{
		// constructors
		explicit Plane(const Vector<n, T> & = NormVector<n, T>(), T = 0);
		template <unsigned m, typename U> explicit Plane(const Plane<m, U> &);
		/**
		 * Create a plane from a surface-normal vector and an arbitrary
		 * spatial-coordinate which intersects with the plane.
		 */
		Plane(const Vector<n, T> &no, const Vector<n, T> &);

		// assignment
		Plane &operator =(const Vector<n, T> &);

		// conversion
		template <typename U> operator Plane<n, U>() const;

		Vector<n, T> no;
		T d;
	};

	// stream insertion/extraction
	template <typename Char, typename CharTraits, unsigned n, typename T> std::basic_ostream<Char, CharTraits> &operator <<(std::basic_ostream<Char, CharTraits> &, const Plane<n, T> &);
	template <typename Char, typename CharTraits, unsigned n, typename T> std::basic_istream<Char, CharTraits> &operator >>(std::basic_istream<Char, CharTraits> &, Plane<n, T> &);

	// standard-library compatibility
	template <unsigned n, typename T> void swap(Plane<n, T> &, Plane<n, T> &);
}}

#	include "Plane.tpp"
#endif
