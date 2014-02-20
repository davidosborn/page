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

#ifndef    page_math_Euler_hpp
#   define page_math_Euler_hpp

#	include <iosfwd> // basic_[io]stream

#	include "ArithmeticConversion.hpp"
#	include "fwd.hpp" // Axan, Euler, Matrix, Quat
#	include "Vector.hpp"

namespace page { namespace math
{
	/**
	 * A euler rotation class.
	 *
	 * The default unit of measurement is radians.
	 */
	template <typename T> struct Euler
	{
		// constructors
		Euler(T = 0, T = 0, T = 0);
		template <typename U> explicit Euler(const Euler<U> &);
		explicit Euler(const Axan<T> &);
		explicit Euler(const Matrix<3, 3, T> &);
		explicit Euler(const Quat<T> &);
		explicit Euler(const Vector<3, T> &);
		Euler(const Vector<3, T> &, const Vector<3, T> &);

		// conversion
		template <typename U> operator Euler<U>() const;
		explicit operator Vector<3, T>() const;

		T yaw, pitch, roll;
	};

	// arithmetic operators
	template <typename T, typename U> Euler<typename ArithmeticConversion<T, U>::Result> operator +(const Euler<T> &, const Euler<U> &);
	template <typename T, typename U> Euler<typename ArithmeticConversion<T, U>::Result> operator -(const Euler<T> &, const Euler<U> &);

	// scalar arithmetic operators
	template <typename T, typename U> Euler<typename ArithmeticConversion<T, U>::Result> operator *(const Euler<T> &, U);
	template <typename T, typename U> Euler<typename ArithmeticConversion<T, U>::Result> operator /(const Euler<T> &, U);
	template <typename T, typename U> Euler<typename ArithmeticConversion<T, U>::Result> operator +(const Euler<T> &, U);
	template <typename T, typename U> Euler<typename ArithmeticConversion<T, U>::Result> operator -(const Euler<T> &, U);

	// relational operators
	template <typename T, typename U> Vector<3, bool> operator <(const Euler<T> &, const Euler<U> &);
	template <typename T, typename U> Vector<3, bool> operator >(const Euler<T> &, const Euler<U> &);
	template <typename T, typename U> Vector<3, bool> operator <=(const Euler<T> &, const Euler<U> &);
	template <typename T, typename U> Vector<3, bool> operator >=(const Euler<T> &, const Euler<U> &);
	template <typename T, typename U> Vector<3, bool> operator ==(const Euler<T> &, const Euler<U> &);
	template <typename T, typename U> Vector<3, bool> operator !=(const Euler<T> &, const Euler<U> &);

	// assignment operators
	template <typename T, typename U> Euler<T> &operator +=(Euler<T> &, const Euler<U> &);
	template <typename T, typename U> Euler<T> &operator -=(Euler<T> &, const Euler<U> &);

	// scalar assignment operators
	template <typename T, typename U> Euler<T> &operator *=(Euler<T> &, U);
	template <typename T, typename U> Euler<T> &operator /=(Euler<T> &, U);
	template <typename T, typename U> Euler<T> &operator +=(Euler<T> &, U);
	template <typename T, typename U> Euler<T> &operator -=(Euler<T> &, U);

	// angle conversions
	template <typename T> Euler<T> DegToRad(const Euler<T> &);
	template <typename T> Euler<T> DegToRev(const Euler<T> &);
	template <typename T> Euler<T> RadToDeg(const Euler<T> &);
	template <typename T> Euler<T> RadToRev(const Euler<T> &);
	template <typename T> Euler<T> RevToDeg(const Euler<T> &);
	template <typename T> Euler<T> RevToRad(const Euler<T> &);

	// vector transformation
	template <typename T, typename U> Vector<3, typename ArithmeticConversion<T, U>::Result> operator *(const Euler<T> &, const Vector<3, U> &);

	// stream insertion/extraction
	template <typename Char, typename CharTraits, typename T> std::basic_ostream<Char, CharTraits> &operator <<(std::basic_ostream<Char, CharTraits> &, const Euler<T> &);
	template <typename Char, typename CharTraits, typename T> std::basic_istream<Char, CharTraits> &operator >>(std::basic_istream<Char, CharTraits> &, Euler<T> &);

	// standard-library compatibility
	template <typename T> void swap(Euler<T> &, Euler<T> &);
}}

#	include "Euler.tpp"
#endif
