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

#ifndef    page_math_OrthoFrustum_hpp
#   define page_math_OrthoFrustum_hpp

#	include <iosfwd> // [io]stream
#	include "Frustum.hpp"
#	include "fwd.hpp" // OrthoFrustum
#	include "Matrix.hpp"
#	include "Quat.hpp"
#	include "Vector.hpp"

namespace page
{
	namespace math
	{
		/**
		 * Orthographic viewing frustum.
		 *
		 * @note The direction vector cuts through the center of the volume.
		 */
		template <typename T> struct OrthoFrustum
		{
			// constructors
			OrthoFrustum(T near, T far, T width = 1, T height = 1, const Vector<3, T> &co = 0, const Quat<T> &dir = Quat<T>());
			OrthoFrustum(T near, T far, T fov, T aspect, const Vector<3, T> &co, const Vector<3, T> &no);
			template <typename U> explicit OrthoFrustum(const OrthoFrustum<U> &);

			// conversion
			template <typename U> operator OrthoFrustum<U>() const;
			explicit operator Matrix<4, 4, T>() const;

			T near, far, width, height;
			Vector<3, T> co; // origin
			Quat<T> dir;
		};

		// matrix conversion
		template <typename T> Matrix<3, 4, T> GetMatrix(const OrthoFrustum<T> &);
		template <typename T> Matrix<3, 4, T> GetProjMatrix(const OrthoFrustum<T> &);
		template <typename T> Matrix<3, 4, T> GetInvProjMatrix(const OrthoFrustum<T> &);
		template <typename T> Matrix<3, 4, T> GetViewMatrix(const OrthoFrustum<T> &);
		template <typename T> Matrix<3, 4, T> GetInvViewMatrix(const OrthoFrustum<T> &);

		// frustum conversion
		template <typename T> Frustum<T> GetFrustum(const OrthoFrustum<T> &);

		// stream insertion/extraction
		template <typename T> std::ostream &operator <<(std::ostream &, const OrthoFrustum<T> &);
		template <typename T> std::istream &operator >>(std::istream &, OrthoFrustum<T> &);

		// specialized algorithms
		template <typename T> void swap(OrthoFrustum<T> &, OrthoFrustum<T> &);
	}
}

#	include "OrthoFrustum.tpp"
#endif
