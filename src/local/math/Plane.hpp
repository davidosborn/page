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

#ifndef    page_local_math_Plane_hpp
#   define page_local_math_Plane_hpp

#	include <iosfwd> // [io]stream
#	include "fwd.hpp" // Plane
#	include "Vector.hpp"

namespace page
{
	namespace math
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
		template <unsigned n, typename T> std::ostream &operator <<(std::ostream &, const Plane<n, T> &);
		template <unsigned n, typename T> std::istream &operator >>(std::istream &, Plane<n, T> &);

		// specialized algorithms
		template <unsigned n, typename T> void swap(Plane<n, T> &, Plane<n, T> &);
	}
}

#	include "Plane.tpp"
#endif
