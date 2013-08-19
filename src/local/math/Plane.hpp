#ifndef    page_math_Plane_hpp
#   define page_math_Plane_hpp

#	include <iosfwd> // basic_[io]stream

#	include "fwd.hpp" // Plane
#	include "Vector.hpp"

namespace page { namespace math
{
	template <unsigned n, typename T> struct Plane
	{
		// constructors
		explicit Plane(const Vector<n, T> & = NormVector<n, T>(), T = 0);
		template <unsigned m, typename U> explicit Plane(const Plane<m, U> &);
		/**
		 * Create a plane from a surface-normal vector and an arbitrary
		 * spatial-coordinate which intersects with the plane.
		 */
		Plane(const Vector<n, T> &no, const Vector<n, T> &);

		// assignment
		Plane &operator =(const Vector<n, T> &);

		// conversion
		template <typename U> operator Plane<n, U>() const;

		Vector<n, T> no;
		T d;
	};

	// stream insertion/extraction
	template <typename Char, typename CharTraits, unsigned n, typename T> std::basic_ostream<Char, CharTraits> &operator <<(std::basic_ostream<Char, CharTraits> &, const Plane<n, T> &);
	template <typename Char, typename CharTraits, unsigned n, typename T> std::basic_istream<Char, CharTraits> &operator >>(std::basic_istream<Char, CharTraits> &, Plane<n, T> &);

	// standard-library compatibility
	template <unsigned n, typename T> void swap(Plane<n, T> &, Plane<n, T> &);
}}

#	include "Plane.tpp"
#endif
