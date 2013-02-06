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

#include <algorithm> // swap
#include <istream>
#include <ostream>

namespace page
{
	namespace math
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
/*			return Frustum(
				Plane(),                            // left
				Plane(),                            // right
				Plane(),                            // bottom
				Plane(),                            // top
				Plane(vf.no, vf.co + vf.no * near), // near
				Plane(vf.no, vf.co + vf.no * far)); // far*/
		}

		// stream insertion/extraction
		template <typename T> std::ostream &operator <<(std::ostream &os, const ViewFrustum<T> &vf)
		{
			// FIXME: implement
			return os;
		}
		template <typename T> std::istream &operator >>(std::istream &is, ViewFrustum<T> &vf)
		{
			// FIXME: implement
			return is;
		}

		// specialized algorithms
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
	}
}
