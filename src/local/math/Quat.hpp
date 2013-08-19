#ifndef    page_math_Quat_hpp
#   define page_math_Quat_hpp

#	include <cstddef> // ptrdiff_t, size_t
#	include <iosfwd> // basic_[io]stream
#	include <iterator> // reverse_iterator

#	include "ArithmeticConversion.hpp"
#	include "fwd.hpp" // Axan, Euler, Matrix, Quat
#	include "Vector.hpp"

namespace page { namespace math
{
	/**
	 * A unit-quaternion class.
	 *
	 * @note Breaking from tradition, w is the last component.
	 */
	template <typename T> struct Quat
	{
		// container traits
		typedef T value_type;
		typedef value_type &reference;
		typedef const value_type &const_reference;
		typedef value_type *iterator;
		typedef const value_type *const_iterator;
		typedef std::reverse_iterator<iterator> reverse_iterator;
		typedef std::reverse_iterator<const_iterator> const_reverse_iterator;
		typedef std::ptrdiff_t difference_type;
		typedef std::size_t size_type;

		// constructors
		Quat();
		Quat(T, T, T);
		Quat(T, T, T, T);
		template <typename U> explicit Quat(const Quat<U> &);
		explicit Quat(const Axan<T> &);
		explicit Quat(const Euler<T> &);
		explicit Quat(const Matrix<3, 3, T> &);
		explicit Quat(const Vector<3, T> &);
		explicit Quat(const Vector<4, T> &);
		Quat(const Vector<3, T> &, const Vector<3, T> &);

		// conversion
		template <typename U> operator Quat<U>() const;
		explicit operator Vector<3, T>() const;
		explicit operator Vector<4, T>() const;

		// iterators
		iterator begin();
		const_iterator begin() const;
		iterator end();
		const_iterator end() const;

		// reverse iterators
		reverse_iterator rbegin();
		const_reverse_iterator rbegin() const;
		reverse_iterator rend();
		const_reverse_iterator rend() const;

		// element access
		reference operator [](size_type);
		const_reference operator [](size_type) const;

		// zero equivalence
		explicit operator bool() const;

		union
		{
			__extension__ struct { T x, y, z, w; };
			T _data[4]; // HACK: conceptually private
		};
	};

	// unary operators
	template <typename T> Quat<T> &operator ++(Quat<T> &);
	template <typename T> Quat<T> &operator --(Quat<T> &);
	template <typename T> Quat<T> operator ++(Quat<T> &, int);
	template <typename T> Quat<T> operator --(Quat<T> &, int);
	template <typename T> Quat<T> operator +(const Quat<T> &);
	template <typename T> Quat<T> operator -(const Quat<T> &);

	// arithmetic operators
	template <typename T, typename U> Quat<typename ArithmeticConversion<T, U>::Result> operator *(const Quat<T> &, const Quat<U> &);
	template <typename T, typename U> Quat<typename ArithmeticConversion<T, U>::Result> operator /(const Quat<T> &, const Quat<U> &);
	template <typename T, typename U> Quat<typename ArithmeticConversion<T, U>::Result> operator +(const Quat<T> &, const Quat<U> &);
	template <typename T, typename U> Quat<typename ArithmeticConversion<T, U>::Result> operator -(const Quat<T> &, const Quat<U> &);

	// scalar arithmetic operators
	template <typename T, typename U> Quat<typename ArithmeticConversion<T, U>::Result> operator *(const Quat<T> &, U);
	template <typename T, typename U> Quat<typename ArithmeticConversion<T, U>::Result> operator /(const Quat<T> &, U);
	template <typename T, typename U> Quat<typename ArithmeticConversion<T, U>::Result> operator +(const Quat<T> &, U);
	template <typename T, typename U> Quat<typename ArithmeticConversion<T, U>::Result> operator -(const Quat<T> &, U);

	// reverse scalar arithmetic operators
	template <typename T, typename U> Quat<typename ArithmeticConversion<T, U>::Result> operator *(T, const Quat<U> &);
	template <typename T, typename U> Quat<typename ArithmeticConversion<T, U>::Result> operator /(T, const Quat<U> &);
	template <typename T, typename U> Quat<typename ArithmeticConversion<T, U>::Result> operator +(T, const Quat<U> &);
	template <typename T, typename U> Quat<typename ArithmeticConversion<T, U>::Result> operator -(T, const Quat<U> &);

	// relational operators
	template <typename T, typename U> bool operator ==(const Quat<T> &, const Quat<U> &);
	template <typename T, typename U> bool operator !=(const Quat<T> &, const Quat<U> &);

	// assignment operators
	template <typename T, typename U> Quat<T> &operator *=(Quat<T> &, const Quat<U> &);
	template <typename T, typename U> Quat<T> &operator /=(Quat<T> &, const Quat<U> &);
	template <typename T, typename U> Quat<T> &operator +=(Quat<T> &, const Quat<U> &);
	template <typename T, typename U> Quat<T> &operator -=(Quat<T> &, const Quat<U> &);

	// scalar assignment operators
	template <typename T, typename U> Quat<T> &operator *=(Quat<T> &, U);
	template <typename T, typename U> Quat<T> &operator /=(Quat<T> &, U);
	template <typename T, typename U> Quat<T> &operator +=(Quat<T> &, U);
	template <typename T, typename U> Quat<T> &operator -=(Quat<T> &, U);

	// other operations
	template <typename T, typename U> Quat<typename ArithmeticConversion<T, U>::Result> Between(const Quat<T> &, const Quat<U> &);
	template <typename T, typename U> typename ArithmeticConversion<T, U>::Result Dot(const Quat<T> &, const Quat<U> &);
	template <typename T> Quat<T> Inv(const Quat<T> &);
	template <typename T> T Len(const Quat<T> &);
	template <typename T, typename U> bool Near(const Quat<T> &, const Quat<U> &);
	template <typename T> Quat<T> Norm(const Quat<T> &);

	// axis-angle transformation
	template <typename T, typename U> Quat<typename ArithmeticConversion<T, U>::Result> operator *(const Quat<T> &, const Axan<U> &);
	template <typename T, typename U> Quat<typename ArithmeticConversion<T, U>::Result> operator /(const Quat<T> &, const Axan<U> &);
	template <typename T, typename U> Quat<T> &operator *=(const Quat<T> &, const Axan<U> &);
	template <typename T, typename U> Quat<T> &operator /=(const Quat<T> &, const Axan<U> &);

	// euler transformation
	template <typename T, typename U> Quat<typename ArithmeticConversion<T, U>::Result> operator *(const Quat<T> &, const Euler<U> &);
	template <typename T, typename U> Quat<typename ArithmeticConversion<T, U>::Result> operator /(const Quat<T> &, const Euler<U> &);
	template <typename T, typename U> Quat<T> &operator *=(const Quat<T> &, const Euler<U> &);
	template <typename T, typename U> Quat<T> &operator /=(const Quat<T> &, const Euler<U> &);

	// matrix transformation
	template <typename T, typename U> Quat<typename ArithmeticConversion<T, U>::Result> operator *(const Quat<T> &, const Matrix<3, 3, U> &);
	template <typename T, typename U> Quat<typename ArithmeticConversion<T, U>::Result> operator /(const Quat<T> &, const Matrix<3, 3, U> &);
	template <typename T, typename U> Quat<T> &operator *=(const Quat<T> &, const Matrix<3, 3, U> &);
	template <typename T, typename U> Quat<T> &operator /=(const Quat<T> &, const Matrix<3, 3, U> &);

	// vector transformation
	template <typename T, typename U> Vector<3, typename ArithmeticConversion<T, U>::Result> operator *(const Quat<T> &, const Vector<3, U> &);

	// stream insertion/extraction
	template <typename Char, typename CharTraits, typename T> std::basic_ostream<Char, CharTraits> &operator <<(std::basic_ostream<Char, CharTraits> &, const Quat<T> &);
	template <typename Char, typename CharTraits, typename T> std::basic_istream<Char, CharTraits> &operator >>(std::basic_istream<Char, CharTraits> &, Quat<T> &);

	// standard-library compatibility
	template <typename T> void swap(Quat<T> &, Quat<T> &);
}}

#	include "Quat.tpp"
#endif
