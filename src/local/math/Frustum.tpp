#include <algorithm> // copy{,_n}, fill, swap_ranges, transform

#include "../util/io/separated_istream_iterator.hpp"
#include "../util/io/separated_ostream_iterator.hpp"
#include "Matrix.hpp"

namespace page { namespace math
{
	// constructors
	template <typename T> Frustum<T>::Frustum(const Plane &p) :
		left(_data[0]), right(_data[1]), bottom(_data[2]), top(_data[3]), near(_data[4]), far(_data[5])
	{
		std::fill(begin(), end(), p);
	}
	template <typename T> Frustum<T>::Frustum(const Plane &left, const Plane &right, const Plane &bottom, const Plane &top, const Plane &near, const Plane &far) :
		left(_data[0]), right(_data[1]), bottom(_data[2]), top(_data[3]), near(_data[4]), far(_data[5]),
		left(left), right(right), bottom(bottom), top(top), near(near), far(far) {}
	template <typename T> Frustum<T>::Frustum(const Frustum &other) :
		left(_data[0]), right(_data[1]), bottom(_data[2]), top(_data[3]), near(_data[4]), far(_data[5])
	{
		std::copy(other.begin(), other.end(), begin());
	}
	template <typename T> template <typename U> Frustum<T>::Frustum(const Frustum<U> &other) :
		left(_data[0]), right(_data[1]), bottom(_data[2]), top(_data[3]), near(_data[4]), far(_data[5])
	{
		std::transform(other.begin(), other.end(), begin(), [](U t){ return static_cast<T>(t); });
	}
	template <typename T> Frustum<T>::Frustum(const Matrix<4, 4, T> &m) :
		left(_data[0]), right(_data[1]), bottom(_data[2]), top(_data[3]), near(_data[4]), far(_data[5])
	{
		// Gribb and Hartmann, Fast Extraction of Viewing Planes, 2001
		left = Plane(Norm(Vector<3, T>(
			m[0][3] + m[0][0],
			m[1][3] + m[1][0],
			m[2][3] + m[2][0])),
			m[3][3] + m[3][0]);
		right = Plane(Norm(Vector<3, T>(
			m[0][3] - m[0][0],
			m[1][3] - m[1][0],
			m[2][3] - m[2][0])),
			m[3][3] - m[3][0]);
		bottom = Plane(Norm(Vector<3, T>(
			m[0][3] + m[0][1],
			m[1][3] + m[1][1],
			m[2][3] + m[2][1])),
			m[3][3] + m[3][1]);
		top = Plane(Norm(Vector<3, T>(
			m[0][3] - m[0][1],
			m[1][3] - m[1][1],
			m[2][3] - m[2][1])),
			m[3][3] - m[3][1]);
		near = Plane(Norm(Vector<3, T>(
			m[0][2],
			m[1][2],
			m[2][2])),
			m[3][2]);
		far = Plane(Norm(Vector<3, T>(
			m[0][3] - m[0][2],
			m[1][3] - m[1][2],
			m[2][3] - m[2][2])),
			m[3][3] - m[3][2]);
	}

	// assignment
	template <typename T> Frustum<T> &Frustum<T>::operator =(const Plane &p)
	{
		std::fill(begin(), end(), p);
		return *this;
	}
	template <typename T> Frustum<T> &Frustum<T>::operator =(const Frustum &other)
	{
		std::copy(other.begin(), other.end(), begin());
		return *this;
	}

	// conversion
	template <typename T> template <typename U> Frustum<T>::operator Frustum<U>() const
	{
		Frustum<U> r;
		std::copy(begin(), end(), r.begin());
		return r;
	}

	// iterators
	template <typename T> typename Frustum<T>::iterator Frustum<T>::begin()
		{ return _data; }
	template <typename T> typename Frustum<T>::const_iterator Frustum<T>::begin() const
		{ return _data; }
	template <typename T> typename Frustum<T>::iterator Frustum<T>::end()
		{ return begin() + 6; }
	template <typename T> typename Frustum<T>::const_iterator Frustum<T>::end() const
		{ return begin() + 6; }

	// reverse iterators
	template <typename T> typename Frustum<T>::reverse_iterator Frustum<T>::rbegin()
		{ return reverse_iterator(begin()); }
	template <typename T> typename Frustum<T>::const_reverse_iterator Frustum<T>::rbegin() const
		{ return const_reverse_iterator(begin()); }
	template <typename T> typename Frustum<T>::reverse_iterator Frustum<T>::rend()
		{ return reverse_iterator(end()); }
	template <typename T> typename Frustum<T>::const_reverse_iterator Frustum<T>::rend() const
		{ return const_reverse_iterator(end()); }

	// element access
	template <typename T> typename Frustum<T>::reference Frustum<T>::operator [](size_type i)
		{ return begin()[i]; }
	template <typename T> typename Frustum<T>::const_reference Frustum<T>::operator [](size_type i) const
		{ return begin()[i]; }

	// stream insertion/extraction
	template <typename Char, typename CharTraits, typename T>
		std::basic_ostream<Char, CharTraits> &operator <<(std::basic_ostream<Char, CharTraits> &os, const Frustum<T> &f)
	{
		os << CharTraits::to_char_type('(');
		std::copy(f.begin(), f.end(),
			util::separated_ostream_iterator<typename Frustum<T>::Plane, Char, CharTraits>(os, CharTraits::to_char_type(',')));
		os << CharTraits::to_char_type(')');
		return os;
	}
	template <typename Char, typename CharTraits, typename T>
		std::basic_istream<Char, CharTraits> &operator >>(std::basic_istream<Char, CharTraits> &is, Frustum<T> &f)
	{
		// read opening bracket
		Char ch;
		is >> ch;
		if (!CharTraits::eq(ch, CharTraits::to_char_type('(')))
			is.setstate(std::ios_base::failbit);

		// read planes
		std::copy_n(
			util::separated_istream_iterator<typename Frustum<T>::Plane, Char, CharTraits>(is, CharTraits::to_char_type(',')),
			6, f.begin());

		// read closing bracket
		is >> ch;
		if (!CharTraits::eq(ch, CharTraits::to_char_type(')')))
			is.setstate(std::ios_base::failbit);

		return is;
	}

	// standard-library compatibility
	template <typename T> void swap(Frustum<T> &a, Frustum<T> &b)
	{
		std::swap_ranges(a.begin(), a.end(), b.begin());
	}
}}
