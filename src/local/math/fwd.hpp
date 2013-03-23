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

#ifndef    page_local_math_fwd_hpp
#   define page_local_math_fwd_hpp

namespace page
{
	namespace math
	{
		/**
		 * The default floating-point type.
		 */
		typedef float defaultType;

		/*-------+
		| colors |
		+-------*/

		template <unsigned, typename = defaultType>
			class Color;

		template <typename = defaultType>
			class HsvColor;

		template <typename = defaultType>
			class RgbColor;

		template <typename = defaultType>
			class RgbaColor;

		template <typename = defaultType>
			class YcbcrColor;

		/*----------------+
		| everything else |
		+----------------*/

		template <unsigned, typename = defaultType>
			class Aabb;

		template <typename = defaultType>
			class Axan;

		template <typename = defaultType>
			class Euler;

		template <typename = defaultType>
			class Frustum;

		template <unsigned nr, unsigned = nr, typename = defaultType>
			class Matrix;

		template <typename = defaultType>
			class OrthoFrustum;

		template <unsigned, typename = defaultType>
			class Plane;

		template <typename = defaultType>
			class Quat;

		template <unsigned, typename>
			class Slice;

		template <unsigned, typename = defaultType>
			class Vector;

		template <typename = defaultType>
			class ViewFrustum;

		/*--------+
		| aliases |
		+--------*/

		typedef Matrix<3> Mat3;
		typedef Matrix<4> Mat4;

		typedef Vector<2> Vec2;
		typedef Vector<3> Vec3;
		typedef Vector<4> Vec4;

		typedef Vector<2, int> Vec2i;
		typedef Vector<3, int> Vec3i;
		typedef Vector<4, int> Vec4i;

		typedef Vector<2, unsigned> Vec2u;
		typedef Vector<3, unsigned> Vec3u;
		typedef Vector<4, unsigned> Vec4u;
	}
}

#endif
