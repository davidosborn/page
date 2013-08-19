#ifndef    page_math_OrthoFrustum_hpp
#   define page_math_OrthoFrustum_hpp

#	include <iosfwd> // basic_[io]stream

#	include "Frustum.hpp"
#	include "fwd.hpp" // OrthoFrustum
#	include "Matrix.hpp"
#	include "Quat.hpp"
#	include "Vector.hpp"

namespace page { namespace math
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
	template <typename Char, typename CharTraits, typename T> std::basic_ostream<Char, CharTraits> &operator <<(std::basic_ostream<Char, CharTraits> &, const OrthoFrustum<T> &);
	template <typename Char, typename CharTraits, typename T> std::basic_istream<Char, CharTraits> &operator >>(std::basic_istream<Char, CharTraits> &, OrthoFrustum<T> &);

	// standard-library compatibility
	template <typename T> void swap(OrthoFrustum<T> &, OrthoFrustum<T> &);
}}

#	include "OrthoFrustum.tpp"
#endif
