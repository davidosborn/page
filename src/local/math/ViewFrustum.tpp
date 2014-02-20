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
	template <typename T> ViewFrustum<T>::ViewFrustum(T near, T far, T fov, T aspect, const Vector<3, T> &co, const Quat<T> &dir) :
		near(near), far(far), fov(fov), aspect(aspect), co(co), dir(dir) {}
	template <typename T> ViewFrustum<T>::ViewFrustum(T near, T far, T fov, T aspect, const Vector<3, T> &co, const Vector<3, T> &no) :
		near(near), far(far), fov(fov), aspect(aspect), co(co), dir(LookMatrix(no)) {}
	template <typename T> template <typename U> ViewFrustum<T>::ViewFrustum(const ViewFrustum<U> &other) :
		near(other.near), far(other.far), fov(other.fov), aspect(other.aspect), co(other.co), dir(other.dir) {}

	// conversion
	template <typename T> template <typename U> ViewFrustum<T>::operator ViewFrustum<U>() const
	{
		return ViewFrustum<U>(near, far, fov, aspect, co, dir);
	}
	template <typename T> ViewFrustum<T>::operator Matrix<4, 4, T>() const
	{
		return GetMatrix(*this);
	}

	// matrix conversion
	template <typename T> Matrix<4, 4, T> GetMatrix(const ViewFrustum<T> &vf)
	{
		return GetProjMatrix(vf) * Matrix<4, 4, T>(GetViewMatrix(vf));
	}
	template <typename T> Matrix<4, 4, T> GetProjMatrix(const ViewFrustum<T> &vf)
	{
		return PerspectiveMatrix(vf.fov, vf.aspect, vf.near, vf.far);
	}
	template <typename T> Matrix<4, 4, T> GetInvProjMatrix(const ViewFrustum<T> &vf)
	{
		return InvPerspectiveMatrix(vf.fov, vf.aspect, vf.near, vf.far);
	}
	template <typename T> Matrix<3, 4, T> GetViewMatrix(const ViewFrustum<T> &vf)
	{
		return Matrix<3, 4, T>(RotationMatrix(Inv(vf.dir))) * TranslationMatrix(-vf.co);
	}
	template <typename T> Matrix<3, 4, T> GetInvViewMatrix(const ViewFrustum<T> &vf)
	{
		return TranslationMatrix(vf.co) * Matrix<3, 4, T>(RotationMatrix(vf.dir));
	}

	// frustum conversion
	template <typename T> Frustum<T> GetFrustum(const ViewFrustum<T> &vf)
	{
		return Frustum<T>(GetMatrix(vf));
		// FIXME: it might be faster to calculate the planes directly
		/*return Frustum(
			Plane(),                            // left
			Plane(),                            // right
			Plane(),                            // bottom
			Plane(),                            // top
			Plane(vf.no, vf.co + vf.no * near), // near
			Plane(vf.no, vf.co + vf.no * far)); // far*/
	}

	// stream insertion/extraction
	template <typename Char, typename CharTraits, typename T> std::basic_ostream<Char, CharTraits> &operator <<(std::basic_ostream<Char, CharTraits> &os, const ViewFrustum<T> &vf)
	{
		return os <<
			CharTraits::to_char_type('(') << vf.near   <<
			CharTraits::to_char_type(',') << vf.far    <<
			CharTraits::to_char_type(',') << vf.fov    <<
			CharTraits::to_char_type(',') << vf.aspect <<
			CharTraits::to_char_type(',') << vf.co     <<
			CharTraits::to_char_type(',') << vf.dir    <<
			CharTraits::to_char_type(')');
	}
	template <typename Char, typename CharTraits, typename T> std::basic_istream<Char, CharTraits> &operator >>(std::basic_istream<Char, CharTraits> &is, ViewFrustum<T> &vf)
	{
		// read opening bracket
		Char ch;
		is >> ch;
		if (!CharTraits::eq(ch, CharTraits::to_char_type('(')))
			is.setstate(std::ios_base::failbit);

		is >> vf.near;

		// read separator
		is >> ch;
		if (!CharTraits::eq(ch, CharTraits::to_char_type(',')))
			is.setstate(std::ios_base::failbit);

		is >> vf.far;

		// read separator
		is >> ch;
		if (!CharTraits::eq(ch, CharTraits::to_char_type(',')))
			is.setstate(std::ios_base::failbit);

		is >> vf.fov;

		// read separator
		is >> ch;
		if (!CharTraits::eq(ch, CharTraits::to_char_type(',')))
			is.setstate(std::ios_base::failbit);

		is >> vf.aspect;

		// read separator
		is >> ch;
		if (!CharTraits::eq(ch, CharTraits::to_char_type(',')))
			is.setstate(std::ios_base::failbit);

		is >> vf.co;

		// read separator
		is >> ch;
		if (!CharTraits::eq(ch, CharTraits::to_char_type(',')))
			is.setstate(std::ios_base::failbit);

		is >> vf.dir;

		// read closing bracket
		is >> ch;
		if (!CharTraits::eq(ch, CharTraits::to_char_type(')')))
			is.setstate(std::ios_base::failbit);

		return is;
	}

	// standard-library compatibility
	template <typename T> void swap(ViewFrustum<T> &a, ViewFrustum<T> &b)
	{
		using std::swap;
		swap(a.near,   b.near);
		swap(a.far,    b.far);
		swap(a.fov,    b.fov);
		swap(a.aspect, b.aspect);
		swap(a.co,     b.co);
		swap(a.dir,    b.dir);
	}
}}
