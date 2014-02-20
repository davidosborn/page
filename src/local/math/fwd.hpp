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

#ifndef    page_math_fwd_hpp
#   define page_math_fwd_hpp

namespace page { namespace math
{
	/**
	 * The default scalar type, which can represent real numbers.
	 */
	typedef float DefaultType;

	/*--------+
	| classes |
	+--------*/

	template <unsigned, typename = DefaultType>
		class Aabb;

	template <unsigned, typename = DefaultType>
		class Array;

	template <typename = DefaultType>
		class Axan;

	template <typename = DefaultType>
		class Euler;

	template <typename = DefaultType>
		class Frustum;

	template <unsigned nr, unsigned = nr, typename = DefaultType>
		class Matrix;

	template <typename = DefaultType>
		class OrthoFrustum;

	template <unsigned, typename = DefaultType>
		class Plane;

	template <typename = DefaultType>
		class Quat;

	template <unsigned, typename>
		class Slice;

	template <unsigned, typename = DefaultType>
		class Vector;

	template <typename = DefaultType>
		class ViewFrustum;

	/*--------------+
	| color classes |
	+--------------*/

	template <unsigned, typename = DefaultType>
		class Color;

	template <typename = DefaultType>
		class HsvColor;

	template <typename = DefaultType>
		class RgbColor;

	template <typename = DefaultType>
		class RgbaColor;

	template <typename = DefaultType>
		class YcbcrColor;

	/*--------+
	| aliases |
	+--------*/

	typedef Matrix<3>    Mat3;
	typedef Matrix<3, 4> Mat34;
	typedef Matrix<4>    Mat4;

	typedef Vector<2> Vec2;
	typedef Vector<3> Vec3;
	typedef Vector<4> Vec4;

	typedef Vector<2, int> Vec2i;
	typedef Vector<3, int> Vec3i;
	typedef Vector<4, int> Vec4i;

	typedef Vector<2, unsigned> Vec2u;
	typedef Vector<3, unsigned> Vec3u;
	typedef Vector<4, unsigned> Vec4u;
}}

#endif
