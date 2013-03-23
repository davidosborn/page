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

#ifndef    page_local_math_Axan_hpp
#   define page_local_math_Axan_hpp

#	include <iosfwd> // [io]stream

#	include "ArithmeticConversion.hpp"
#	include "fwd.hpp" // Axan, Euler, Matrix, Quat, Vector
#	include "Vector.hpp"

namespace page
{
	namespace math
	{
		/**
		 * An axis/angle rotation class.
		 *
		 * The axis is a unit-length vector.  The angle is measured in radians.
		 */
		template <typename T> struct Axan
		{
			// constructors
			Axan(T, T, T, T = 0);
			explicit Axan(const Vector<3, T> & = NormVector<3, T>(), T = 0);
			template <typename U> explicit Axan(const Axan<U> &);
			explicit Axan(const Euler<T> &);
			explicit Axan(const Matrix<3, 3, T> &);
			explicit Axan(const Quat<T> &);
			explicit Axan(const Vector<4, T> &);
			Axan(const Vector<3, T> &, const Vector<3, T> &);

			// conversion
			template <typename U> operator Axan<U>() const;

			Vector<3, T> axis;
			T angle;
		};

		// relational operators
		template <typename T, typename U> bool operator ==(const Axan<T> &, const Axan<U> &);
		template <typename T, typename U> bool operator !=(const Axan<T> &, const Axan<U> &);

		// other operations
		template <typename T, typename U> Axan<typename ArithmeticConversion<T, U>::Result> Between(const Axan<T> &, const Axan<U> &);
		template <typename T, typename U> bool Near(const Axan<T> &, const Axan<U> &);

		// vector transformation
		template <typename T, typename U> Vector<3, typename ArithmeticConversion<T, U>::Result> operator *(const Axan<T> &, const Vector<3, U> &);

		// stream insertion/extraction
		template <typename T> std::ostream &operator <<(std::ostream &, const Axan<T> &);
		template <typename T> std::istream &operator >>(std::istream &, Axan<T> &);

		// specialized algorithms
		template <typename T> void swap(Axan<T> &, Axan<T> &);
	}
}

#	include "Axan.tpp"
#endif
