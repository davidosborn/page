#include <algorithm> // copy{,_n}, transform
#include <functional> // bind2nd, divides, minus, multiplies, plus, negate
#include <utility> // forward

#include <boost/range/iterator_range.hpp>

#include "../util/functional/operator.hpp" // modulus
#include "../util/io/separated_istream_iterator.hpp"
#include "../util/io/separated_ostream_iterator.hpp"

namespace page { namespace math
{
////////// BasicSlice //////////////////////////////////////////////////////////

	/// constructors
	template <unsigned n, typename Iterator>
		BasicSlice<n, Iterator>::BasicSlice(const iterator &iter) :
			iter(iter) {}

	template <unsigned n, typename Iterator> template <typename Iterator2>
		BasicSlice<n, Iterator>::BasicSlice(const BasicSlice<n, Iterator2> &other) :
			iter(other.iter) {}

	/// assignment
	template <unsigned n, typename Iterator>
		const BasicSlice<n, Iterator> &BasicSlice<n, Iterator>::operator =(const value_type &t) const
	{
		std::fill(begin(), end(), t);
		return *this;
	}

	template <unsigned n, typename Iterator>
		const BasicSlice<n, Iterator> &BasicSlice<n, Iterator>::operator =(const Vector<n, value_type> &v) const
	{
		std::copy(v.begin(), v.end(), begin());
		return *this;
	}

	template <unsigned n, typename Iterator>
		const BasicSlice<n, Iterator> &BasicSlice<n, Iterator>::operator =(const BasicSlice &other) const
	{
		std::copy(other.begin(), other.end(), begin());
		return *this;
	}

	template <unsigned n, typename Iterator> template <typename Iterator2>
		const BasicSlice<n, Iterator> &BasicSlice<n, Iterator>::operator =(const BasicSlice<n, Iterator2> &other) const
	{
		std::copy(other.begin(), other.end(), begin());
		return *this;
	}

	/// conversion
	template <unsigned n, typename Iterator>
		BasicSlice<n, Iterator>::operator Vector<n, value_type>() const
	{
		Vector<n, value_type> r;
		std::copy(begin(), end(), r.begin());
		return r;
	}

	/// iterators
	template <unsigned n, typename Iterator>
		auto BasicSlice<n, Iterator>::begin() const noexcept -> const iterator &
			{ return iter; }

	template <unsigned n, typename Iterator>
		auto BasicSlice<n, Iterator>::end() const -> iterator
			{ return begin() + n; }

	template <unsigned n, typename Iterator>
		auto BasicSlice<n, Iterator>::rbegin() const -> reverse_iterator
			{ return reverse_iterator(end()); }

	template <unsigned n, typename Iterator>
		auto BasicSlice<n, Iterator>::rend() const -> reverse_iterator
			{ return reverse_iterator(begin()); }

	/// element access
	template <unsigned n, typename Iterator>
		auto BasicSlice<n, Iterator>::operator [](size_type i) const -> reference
			{ return begin()[i]; }

////////////////////////////////////////////////////////////////////////////////

	/// unary operators
	template <unsigned n, typename Iterator>
		const Slice<n, Iterator> &operator ++(const Slice<n, Iterator> &s)
	{
		std::transform(s.begin(), s.end(), s.begin(),
			std::bind(std::plus<typename Slice<n, Iterator>::value_type>(), std::placeholders::_1, 1));
		return s;
	}

	template <unsigned n, typename Iterator>
		const Slice<n, Iterator> &operator --(const Slice<n, Iterator> &s)
	{
		std::transform(s.begin(), s.end(), s.begin(),
			std::bind(std::minus<typename Slice<n, Iterator>::value_type>(), std::placeholders::_1, 1));
		return s;
	}

	template <unsigned n, typename Iterator>
		Vector<n, typename Slice<n, Iterator>::value_type> operator ++(const Slice<n, Iterator> &s, int)
	{
		typedef typename Slice<n, Iterator>::value_type T;
		Vector<n, T> r;
		std::transform(s.begin(), s.end(), r.begin(),
			std::bind(std::plus<T>(), std::placeholders::_1, 1));
		return r;
	}

	template <unsigned n, typename Iterator>
		Vector<n, typename Slice<n, Iterator>::value_type> operator --(const Slice<n, Iterator> &s, int)
	{
		typedef typename Slice<n, Iterator>::value_type T;
		Vector<n, T> r;
		std::transform(s.begin(), s.end(), r.begin(),
			std::bind(std::minus<T>(), std::placeholders::_1, 1));
		return r;
	}

	template <unsigned n, typename Iterator>
		Vector<n, typename Slice<n, Iterator>::value_type> operator +(const Slice<n, Iterator> &s)
	{
		return s;
	}

	template <unsigned n, typename Iterator>
		Vector<n, typename Slice<n, Iterator>::value_type> operator -(const Slice<n, Iterator> &s)
	{
		typedef typename Slice<n, Iterator>::value_type T;
		Vector<n, T> r;
		std::transform(s.begin(), s.end(), r.begin(), std::negate<T>());
		return r;
	}

	/// assignment operators
	template <unsigned n, typename Iterator, typename Iterator2>
		const Slice<n, Iterator> &operator *=(const Slice<n, Iterator> &s1, const Slice<n, Iterator2> &s2)
	{
		std::transform(s1.begin(), s1.end(), s2.begin(), s1.begin(),
			std::multiplies<typename Slice<n, Iterator>::value_type>());
		return s1;
	}

	template <unsigned n, typename Iterator, typename Iterator2>
		const Slice<n, Iterator> &operator /=(const Slice<n, Iterator> &s1, const Slice<n, Iterator2> &s2)
	{
		std::transform(s1.begin(), s1.end(), s2.begin(), s1.begin(),
			std::multiplies<typename Slice<n, Iterator>::value_type>());
		return s1;
	}

	template <unsigned n, typename Iterator, typename Iterator2>
		const Slice<n, Iterator> &operator %=(const Slice<n, Iterator> &s1, const Slice<n, Iterator2> &s2)
	{
		std::transform(s1.begin(), s1.end(), s2.begin(), s1.begin(),
			modulus<typename Slice<n, Iterator>::value_type>());
		return s1;
	}

	template <unsigned n, typename Iterator, typename Iterator2>
		const Slice<n, Iterator> &operator +=(const Slice<n, Iterator> &s1, const Slice<n, Iterator2> &s2)
	{
		std::transform(s1.begin(), s1.end(), s2.begin(), s1.begin(),
			std::plus<typename Slice<n, Iterator>::value_type>());
		return s1;
	}

	template <unsigned n, typename Iterator, typename Iterator2>
		const Slice<n, Iterator> &operator -=(const Slice<n, Iterator> &s1, const Slice<n, Iterator2> &s2)
	{
		std::transform(s1.begin(), s1.end(), s2.begin(), s1.begin(),
			std::minus<typename Slice<n, Iterator>::value_type>());
		return s1;
	}

	/// vector assignment operators
	template <unsigned n, typename Iterator, typename T>
		const Slice<n, Iterator> &operator *=(const Slice<n, Iterator> &s, const Vector<n, T> &v)
	{
		std::transform(s.begin(), s.end(), v.begin(), s.begin(),
			std::multiplies<typename Slice<n, Iterator>::value_type>());
		return s;
	}

	template <unsigned n, typename Iterator, typename T>
		const Slice<n, Iterator> &operator /=(const Slice<n, Iterator> &s, const Vector<n, T> &v)
	{
		std::transform(s.begin(), s.end(), v.begin(), s.begin(),
			std::divides<typename Slice<n, Iterator>::value_type>());
		return s;
	}

	template <unsigned n, typename Iterator, typename T>
		const Slice<n, Iterator> &operator %=(const Slice<n, Iterator> &s, const Vector<n, T> &v)
	{
		std::transform(s.begin(), s.end(), v.begin(), s.begin(),
			modulus<typename Slice<n, Iterator>::value_type>());
		return s;
	}

	template <unsigned n, typename Iterator, typename T>
		const Slice<n, Iterator> &operator +=(const Slice<n, Iterator> &s, const Vector<n, T> &v)
	{
		std::transform(s.begin(), s.end(), v.begin(), s.begin(),
			std::plus<typename Slice<n, Iterator>::value_type>());
		return s;
	}

	template <unsigned n, typename Iterator, typename T>
		const Slice<n, Iterator> &operator -=(const Slice<n, Iterator> &s, const Vector<n, T> &v)
	{
		std::transform(s.begin(), s.end(), v.begin(), s.begin(),
			std::minus<typename Slice<n, Iterator>::value_type>());
		return s;
	}

	/// scalar assignment operators
	template <unsigned n, typename Iterator, typename T>
		const Slice<n, Iterator> &operator *=(const Slice<n, Iterator> &s, T t)
	{
		std::transform(s.begin(), s.end(), s.begin(),
			std::bind2nd(std::multiplies<typename Slice<n, Iterator>::value_type>(), t));
		return s;
	}

	template <unsigned n, typename Iterator, typename T>
		const Slice<n, Iterator> &operator /=(const Slice<n, Iterator> &s, T t)
	{
		std::transform(s.begin(), s.end(), s.begin(),
			std::bind2nd(std::divides<typename Slice<n, Iterator>::value_type>(), t));
		return s;
	}

	template <unsigned n, typename Iterator, typename T>
		const Slice<n, Iterator> &operator %=(const Slice<n, Iterator> &s, T t)
	{
		std::transform(s.begin(), s.end(), s.begin(),
			std::bind2nd(modulus<typename Slice<n, Iterator>::value_type>(), t));
		return s;
	}

	template <unsigned n, typename Iterator, typename T>
		const Slice<n, Iterator> &operator +=(const Slice<n, Iterator> &s, T t)
	{
		std::transform(s.begin(), s.end(), s.begin(),
			std::bind2nd(std::plus<typename Slice<n, Iterator>::value_type>(), t));
		return s;
	}

	template <unsigned n, typename Iterator, typename T>
		const Slice<n, Iterator> &operator -=(const Slice<n, Iterator> &s, T t)
	{
		std::transform(s.begin(), s.end(), s.begin(),
			std::bind2nd(std::minus<typename Slice<n, Iterator>::value_type>(), t));
		return s;
	}

	/// stream insertion & extraction
	template <typename Char, typename CharTraits, unsigned n, typename Iterator>
		std::basic_ostream<Char, CharTraits> &operator <<(std::basic_ostream<Char, CharTraits> &os, const Slice<n, Iterator> &s)
	{
		os << CharTraits::to_char_type('(');
		std::copy(s.begin(), s.end(),
			util::separated_ostream_iterator<typename Slice<n, Iterator>::value_type, Char, CharTraits>(os, CharTraits::to_char_type(',')));
		os << CharTraits::to_char_type(')');
		return os;
	}

	template <typename Char, typename CharTraits, unsigned n, typename Iterator>
		std::basic_istream<Char, CharTraits> &operator >>(std::basic_istream<Char, CharTraits> &is, const Slice<n, Iterator> &s)
	{
		// read opening bracket
		Char ch;
		is >> ch;
		if (!CharTraits::eq(ch, CharTraits::to_char_type('(')))
			is.setstate(std::ios_base::failbit);

		// read components
		std::copy_n(
			util::separated_istream_iterator<typename Slice<n, Iterator>::value_type, Char, CharTraits>(is, CharTraits::to_char_type(',')),
			n, s.begin());

		// read closing bracket
		is >> ch;
		if (!CharTraits::eq(ch, CharTraits::to_char_type(')')))
			is.setstate(std::ios_base::failbit);

		return is;
	}
}}
