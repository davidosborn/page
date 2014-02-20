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
	template <typename T> OrthoFrustum<T>::OrthoFrustum(T near, T far, T width, T height, const Vector<3, T> &co, const Quat<T> &dir) :
		near(near), far(far), width(width), height(height), co(co), dir(dir) {}
	template <typename T> OrthoFrustum<T>::OrthoFrustum(T near, T far, T width, T height, const Vector<3, T> &co, const Vector<3, T> &no) :
		near(near), far(far), width(width), height(height), co(co), dir(LookMatrix(no)) {}
	template <typename T> template <typename U> OrthoFrustum<T>::OrthoFrustum(const OrthoFrustum<U> &other) :
		near(other.near), far(other.far), width(other.width), height(other.height), co(other.co), dir(other.dir) {}

	// conversion
	template <typename T> template <typename U> OrthoFrustum<T>::operator OrthoFrustum<U>() const
	{
		return OrthoFrustum<U>(near, far, width, height, co, dir);
	}
	template <typename T> OrthoFrustum<T>::operator Matrix<4, 4, T>() const
	{
		return GetMatrix(*this);
	}

	// matrix conversion
	template <typename T> Matrix<3, 4, T> GetMatrix(const OrthoFrustum<T> &of)
	{
		return GetProjMatrix(of) * GetViewMatrix(of);
	}
	template <typename T> Matrix<3, 4, T> GetProjMatrix(const OrthoFrustum<T> &of)
	{
		return OrthoMatrix(-of.width / 2, of.width / 2, -of.height / 2, of.height / 2, of.near, of.far);
	}
	template <typename T> Matrix<3, 4, T> GetInvProjMatrix(const OrthoFrustum<T> &of)
	{
		return InvOrthoMatrix(-of.width / 2, of.width / 2, -of.height / 2, of.height / 2, of.near, of.far);
	}
	template <typename T> Matrix<3, 4, T> GetViewMatrix(const OrthoFrustum<T> &of)
	{
		return Matrix<3, 4, T>(RotationMatrix(Inv(of.dir))) * TranslationMatrix(-of.co);
	}
	template <typename T> Matrix<3, 4, T> GetInvViewMatrix(const OrthoFrustum<T> &of)
	{
		return TranslationMatrix(of.co) * Matrix<3, 4, T>(RotationMatrix(of.dir));
	}

	// frustum conversion
	template <typename T> Frustum<T> GetFrustum(const OrthoFrustum<T> &of)
	{
		// FIXME: it might be faster to calculate the planes directly
		return Frustum<T>(GetMatrix(of));
	}

	// stream insertion/extraction
	template <typename Char, typename CharTraits, typename T>
		std::basic_ostream<Char, CharTraits> &operator <<(std::basic_ostream<Char, CharTraits> &os, const OrthoFrustum<T> &of)
	{
		return os <<
			CharTraits::to_char_type('(') << of.near   <<
			CharTraits::to_char_type(',') << of.far    <<
			CharTraits::to_char_type(',') << of.width  <<
			CharTraits::to_char_type(',') << of.height <<
			CharTraits::to_char_type(',') << of.co     <<
			CharTraits::to_char_type(',') << of.dir    <<
			CharTraits::to_char_type(')');
	}
	template <typename Char, typename CharTraits, typename T>
		std::basic_istream<Char, CharTraits> &operator >>(std::basic_istream<Char, CharTraits> &is, OrthoFrustum<T> &of)
	{
		// read opening bracket
		Char ch;
		is >> ch;
		if (!CharTraits::eq(ch, CharTraits::to_char_type('(')))
			is.setstate(std::ios_base::failbit);

		is >> of.near;

		// read separator
		is >> ch;
		if (!CharTraits::eq(ch, CharTraits::to_char_type(',')))
			is.setstate(std::ios_base::failbit);

		is >> of.far;

		// read separator
		is >> ch;
		if (!CharTraits::eq(ch, CharTraits::to_char_type(',')))
			is.setstate(std::ios_base::failbit);

		is >> of.width;

		// read separator
		is >> ch;
		if (!CharTraits::eq(ch, CharTraits::to_char_type(',')))
			is.setstate(std::ios_base::failbit);

		is >> of.height;

		// read separator
		is >> ch;
		if (!CharTraits::eq(ch, CharTraits::to_char_type(',')))
			is.setstate(std::ios_base::failbit);

		is >> of.co;

		// read separator
		is >> ch;
		if (!CharTraits::eq(ch, CharTraits::to_char_type(',')))
			is.setstate(std::ios_base::failbit);

		is >> of.dir;

		// read closing bracket
		is >> ch;
		if (!CharTraits::eq(ch, CharTraits::to_char_type(')')))
			is.setstate(std::ios_base::failbit);

		return is;
	}

	// standard-library compatibility
	template <typename T> void swap(OrthoFrustum<T> &a, OrthoFrustum<T> &b)
	{
		using std::swap;
		swap(a.near,   b.near);
		swap(a.far,    b.far);
		swap(a.width,  b.width);
		swap(a.height, b.height);
		swap(a.co,     b.co);
		swap(a.dir,    b.dir);
	}
}}
