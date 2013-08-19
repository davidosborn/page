#ifndef    page_math_fwd_hpp
#   define page_math_fwd_hpp

namespace page { namespace math
{
	/**
	 * The default scalar type, which can represent real numbers.
	 */
	typedef float DefaultType;

	/// classes
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

	/// color classes
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

	/// aliases
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
}}

#endif
