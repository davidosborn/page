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

#ifndef    page_local_math_ViewFrustum_hpp
#   define page_local_math_ViewFrustum_hpp

#	include <iosfwd> // [io]stream

#	include "float.hpp" // Pi
#	include "Frustum.hpp"
#	include "fwd.hpp" // ViewFrustum
#	include "Matrix.hpp"
#	include "Quat.hpp"
#	include "Vector.hpp"

namespace page
{
	namespace math
	{
		/**
		 * Limited viewing frustum.
		 *
		 * @note The near and far planes are parallel.
		 * @note The sides form isosceles trapezoids.
		 * @note The direction vector cuts through the center of the volume.
		 */
		template <typename T> struct ViewFrustum
		{
			// constructors
			ViewFrustum(T near, T far, T fov = Pi<T>() / 2, T aspect = 1, const Vector<3, T> &co = 0, const Quat<T> &dir = Quat<T>());
			ViewFrustum(T near, T far, T fov, T aspect, const Vector<3, T> &co, const Vector<3, T> &no);
			template <typename U> explicit ViewFrustum(const ViewFrustum<U> &);

			// conversion
			template <typename U> operator ViewFrustum<U>() const;
			explicit operator Matrix<4, 4, T>() const;

			T near, far, fov, aspect;
			Vector<3, T> co; // origin
			Quat<T> dir;
		};

		// matrix conversion
		template <typename T> Matrix<4, 4, T> GetMatrix(const ViewFrustum<T> &);
		template <typename T> Matrix<4, 4, T> GetProjMatrix(const ViewFrustum<T> &);
		template <typename T> Matrix<4, 4, T> GetInvProjMatrix(const ViewFrustum<T> &);
		template <typename T> Matrix<3, 4, T> GetViewMatrix(const ViewFrustum<T> &);
		template <typename T> Matrix<3, 4, T> GetInvViewMatrix(const ViewFrustum<T> &);

		// frustum conversion
		template <typename T> Frustum<T> GetFrustum(const ViewFrustum<T> &);

		// stream insertion/extraction
		template <typename T> std::ostream &operator <<(std::ostream &, const ViewFrustum<T> &);
		template <typename T> std::istream &operator >>(std::istream &, ViewFrustum<T> &);

		// specialized algorithms
		template <typename T> void swap(ViewFrustum<T> &, ViewFrustum<T> &);
	}
}

#	include "ViewFrustum.tpp"
#endif
