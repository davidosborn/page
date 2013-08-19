#ifndef    page_math_ViewFrustum_hpp
#   define page_math_ViewFrustum_hpp

#	include <iosfwd> // basic_[io]stream

#	include "float.hpp" // Pi
#	include "Frustum.hpp"
#	include "fwd.hpp" // ViewFrustum
#	include "Matrix.hpp"
#	include "Quat.hpp"
#	include "Vector.hpp"

namespace page { namespace math
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
	template <typename Char, typename CharTraits, typename T> std::basic_ostream<Char, CharTraits> &operator <<(std::basic_ostream<Char, CharTraits> &, const ViewFrustum<T> &);
	template <typename Char, typename CharTraits, typename T> std::basic_istream<Char, CharTraits> &operator >>(std::basic_istream<Char, CharTraits> &, ViewFrustum<T> &);

	// standard-library compatibility
	template <typename T> void swap(ViewFrustum<T> &, ViewFrustum<T> &);
}}

#	include "ViewFrustum.tpp"
#endif
