#ifndef    page_math_Axan_hpp
#   define page_math_Axan_hpp

#	include <iosfwd> // basic_[io]stream

#	include "ArithmeticConversion.hpp"
#	include "fwd.hpp" // Axan, Euler, Matrix, Quat, Vector
#	include "Vector.hpp"

namespace page { namespace math
{
	/**
	 * An axis/angle rotation class.
	 *
	 * The axis is a unit-length vector.  The angle is measured in radians.
	 */
	template <typename T> struct Axan
	{
		// constructors
		Axan(T, T, T, T = 0);
		explicit Axan(const Vector<3, T> & = NormVector<3, T>(), T = 0);
		template <typename U> explicit Axan(const Axan<U> &);
		explicit Axan(const Euler<T> &);
		explicit Axan(const Matrix<3, 3, T> &);
		explicit Axan(const Quat<T> &);
		explicit Axan(const Vector<4, T> &);
		Axan(const Vector<3, T> &, const Vector<3, T> &);

		// conversion
		template <typename U> operator Axan<U>() const;

		Vector<3, T> axis;
		T angle;
	};

	// relational operators
	template <typename T, typename U> bool operator ==(const Axan<T> &, const Axan<U> &);
	template <typename T, typename U> bool operator !=(const Axan<T> &, const Axan<U> &);

	// other operations
	template <typename T, typename U> Axan<typename ArithmeticConversion<T, U>::Result> Between(const Axan<T> &, const Axan<U> &);
	template <typename T, typename U> bool Near(const Axan<T> &, const Axan<U> &);

	// vector transformation
	template <typename T, typename U> Vector<3, typename ArithmeticConversion<T, U>::Result> operator *(const Axan<T> &, const Vector<3, U> &);

	// stream insertion/extraction
	template <typename Char, typename CharTraits, typename T> std::basic_ostream<Char, CharTraits> &operator <<(std::basic_ostream<Char, CharTraits> &, const Axan<T> &);
	template <typename Char, typename CharTraits, typename T> std::basic_istream<Char, CharTraits> &operator >>(std::basic_istream<Char, CharTraits> &, Axan<T> &);

	// standard-library compatibility
	template <typename T> void swap(Axan<T> &, Axan<T> &);
}}

#	include "Axan.tpp"
#endif
