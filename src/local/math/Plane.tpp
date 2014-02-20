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

#include <algorithm> // swap

namespace page { namespace math
{
	// constructors
	template <unsigned n, typename T> Plane<n, T>::Plane(const Vector<n, T> &no, T d) :
		no(no), d(d) {}
	template <unsigned n, typename T> template <unsigned m, typename U> Plane<n, T>::Plane(const Plane<m, U> &other) :
		no(other.no), d(other.d) {}
	template <unsigned n, typename T> Plane<n, T>::Plane(const Vector<n, T> &no, const Vector<n, T> &v) :
		no(no), d(Dot(no, v)) {}

	// assignment
	template <unsigned n, typename T> Plane<n, T> &Plane<n, T>::operator =(const Vector<n, T> &no)
	{
		no = no;
		d  = 0;
		return *this;
	}

	// conversion
	template <unsigned n, typename T> template <typename U> Plane<n, T>::operator Plane<n, U>() const
	{
		return Plane<n, U>(no, d);
	}

	// stream insertion/extraction
	template <typename Char, typename CharTraits, unsigned n, typename T>
		std::basic_ostream<Char, CharTraits> &operator <<(std::basic_ostream<Char, CharTraits> &os, const Plane<n, T> &p)
	{
		return os <<
			CharTraits::to_char_type('(') << p.no <<
			CharTraits::to_char_type(',') << p.d  <<
			CharTraits::to_char_type(')');
	}
	template <typename Char, typename CharTraits, unsigned n, typename T>
		std::basic_istream<Char, CharTraits> &operator >>(std::basic_istream<Char, CharTraits> &is, Plane<n, T> &p)
	{
		// read opening bracket
		Char ch;
		is >> ch;
		if (!CharTraits::eq(ch, CharTraits::to_char_type('(')))
			is.setstate(std::ios_base::failbit);

		is >> p.no;

		// read separator
		is >> ch;
		if (!CharTraits::eq(ch, CharTraits::to_char_type(',')))
			is.setstate(std::ios_base::failbit);

		is >> p.d;

		// read closing bracket
		is >> ch;
		if (!CharTraits::eq(ch, CharTraits::to_char_type(')')))
			is.setstate(std::ios_base::failbit);

		return is;
	}

	// standard-library compatibility
	template <unsigned n, typename T> void swap(Plane<n, T> &a, Plane<n, T> &b)
	{
		using std::swap;
		swap(a.no, b.no);
		swap(a.d,  b.d);
	}
}}
