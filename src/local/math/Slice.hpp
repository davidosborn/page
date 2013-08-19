#ifndef    page_math_Slice_hpp
#   define page_math_Slice_hpp

#	include <cstddef> // size_t
#	include <iosfwd> // basic_[io]stream
#	include <iterator> // iterator_traits, reverse_iterator
#	include <type_traits> // add_const, make_unsigned

#	include "ArithmeticConversion.hpp"
#	include "Vector.hpp"

namespace page { namespace math
{
////////// BasicSlice //////////////////////////////////////////////////////////

	/**
	 * The base class of @c Slice which implements the parts that are common to
	 * all specializations.
	 *
	 * @note This class uses the <em>proxy pattern</em>.  The constness of the
	 *       proxy affects only the operations that modify the state of the
	 *       proxy, not those that modify the state of the targeted object.
	 */
	template <unsigned n, typename Iterator>
		class BasicSlice
	{
		template <unsigned, typename>
			friend class BasicSlice;

		/// container traits
		public:
		using value_type             = typename std::iterator_traits<Iterator>::value_type;
		using reference              = typename std::iterator_traits<Iterator>::reference;
		using const_reference        = typename std::add_const<reference>::type;
		using iterator               = Iterator;
		using const_iterator         = Iterator;
		using reverse_iterator       = std::reverse_iterator<iterator>;
		using const_reverse_iterator = std::reverse_iterator<const_iterator>;
		using difference_type        = typename std::iterator_traits<Iterator>::difference_type;
		using size_type              = typename std::make_unsigned<difference_type>::type;

		/// constructors
		public:
		explicit BasicSlice(const iterator &);

		template <typename Iterator2>
			BasicSlice(const BasicSlice<n, Iterator2> &);

		/// assignment
		const BasicSlice &operator =(const value_type &) const;

		const BasicSlice &operator =(const Vector<n, value_type> &) const;

		const BasicSlice &operator =(const BasicSlice &) const;

		template <typename Iterator2>
			const BasicSlice &operator =(const BasicSlice<n, Iterator2> &) const;

		/// conversion
		public:
		operator Vector<n, value_type>() const;

		/// iterators
		iterator const&  begin()  const noexcept;
		iterator         end()    const;
		reverse_iterator rbegin() const;
		reverse_iterator rend()   const;

		/// element access
		reference operator [](size_type) const;

		/// data members
		private:
		const Iterator iter;
	};

////////// Slice ///////////////////////////////////////////////////////////////

	/**
	 * A slice of a matrix.
	 *
	 * @note This class uses the <em>proxy pattern</em>.  The constness of the
	 *       proxy affects only the operations that modify the state of the
	 *       proxy, not those that modify the state of the targeted object.
	 */
	template <unsigned n, typename Iterator>
		class Slice : public BasicSlice<n, Iterator>
	{
		typedef BasicSlice<n, Iterator> Base;

		public:
		using Base::BasicSlice;
		using Base::operator =;
	};

////////// Slice (2 component) /////////////////////////////////////////////////

	/**
	 * A 2-component slice of a matrix.
	 *
	 * @note This class uses the <em>proxy pattern</em>.  The constness of the
	 *       proxy affects only the operations that modify the state of the
	 *       proxy, not those that modify the state of the targeted object.
	 */
	template <typename Iterator>
		class Slice<2, Iterator> : public BasicSlice<2, Iterator>
	{
		typedef BasicSlice<2, Iterator> Base;

		public:
		using Base::BasicSlice;
		using Base::operator =;

		typename Base::reference
			x = this->begin()[0],
			y = this->begin()[1];
	};

////////// Slice (3 component) /////////////////////////////////////////////////

	/**
	 * A 3-component slice of a matrix.
	 *
	 * @note This class uses the <em>proxy pattern</em>.  The constness of the
	 *       proxy affects only the operations that modify the state of the
	 *       proxy, not those that modify the state of the targeted object.
	 */
	template <typename Iterator>
		class Slice<3, Iterator> : public BasicSlice<3, Iterator>
	{
		typedef BasicSlice<3, Iterator> Base;

		public:
		using Base::BasicSlice;
		using Base::operator =;

		typename Base::reference
			x = this->begin()[0],
			y = this->begin()[1],
			z = this->begin()[2];
	};

////////// Slice (4 component) /////////////////////////////////////////////////

	/**
	 * A 4-component slice of a matrix.
	 *
	 * @note This class uses the <em>proxy pattern</em>.  The constness of the
	 *       proxy affects only the operations that modify the state of the
	 *       proxy, not those that modify the state of the targeted object.
	 */
	template <typename Iterator>
		class Slice<4, Iterator> : public BasicSlice<4, Iterator>
	{
		typedef BasicSlice<4, Iterator> Base;

		public:
		using Base::BasicSlice;
		using Base::operator =;

		typename Base::reference
			x = this->begin()[0],
			y = this->begin()[1],
			z = this->begin()[2],
			w = this->begin()[3];
	};

////////////////////////////////////////////////////////////////////////////////

	/// unary operators
	template <unsigned n, typename Iterator>
		Slice<n, Iterator> &operator ++(Slice<n, Iterator> &);

	template <unsigned n, typename Iterator>
		Slice<n, Iterator> &operator --(Slice<n, Iterator> &);

	template <unsigned n, typename Iterator>
		Vector<n, typename Slice<n, Iterator>::value_type> operator ++(Slice<n, Iterator> &, int);

	template <unsigned n, typename Iterator>
		Vector<n, typename Slice<n, Iterator>::value_type> operator --(Slice<n, Iterator> &, int);

	template <unsigned n, typename Iterator>
		Vector<n, typename Slice<n, Iterator>::value_type> operator +(const Slice<n, Iterator> &);

	template <unsigned n, typename Iterator>
		Vector<n, typename Slice<n, Iterator>::value_type> operator -(const Slice<n, Iterator> &);

	/// assignment operators
	template <unsigned n, typename Iterator, typename Iterator2>
		Slice<n, Iterator> &operator +=(Slice<n, Iterator> &, const Slice<n, Iterator2> &);

	template <unsigned n, typename Iterator, typename Iterator2>
		Slice<n, Iterator> &operator -=(Slice<n, Iterator> &, const Slice<n, Iterator2> &);

	template <unsigned n, typename Iterator, typename Iterator2>
		Slice<n, Iterator> &operator *=(Slice<n, Iterator> &, const Slice<n, Iterator2> &);

	template <unsigned n, typename Iterator, typename Iterator2>
		Slice<n, Iterator> &operator /=(Slice<n, Iterator> &, const Slice<n, Iterator2> &);

	template <unsigned n, typename Iterator, typename Iterator2>
		Slice<n, Iterator> &operator %=(Slice<n, Iterator> &, const Slice<n, Iterator2> &);

	/// vector assignment operators
	template <unsigned n, typename Iterator, typename T>
		Slice<n, Iterator> &operator +=(Slice<n, Iterator> &, const Vector<n, T> &);

	template <unsigned n, typename Iterator, typename T>
		Slice<n, Iterator> &operator -=(Slice<n, Iterator> &, const Vector<n, T> &);

	template <unsigned n, typename Iterator, typename T>
		Slice<n, Iterator> &operator *=(Slice<n, Iterator> &, const Vector<n, T> &);

	template <unsigned n, typename Iterator, typename T>
		Slice<n, Iterator> &operator /=(Slice<n, Iterator> &, const Vector<n, T> &);

	template <unsigned n, typename Iterator, typename T>
		Slice<n, Iterator> &operator %=(Slice<n, Iterator> &, const Vector<n, T> &);

	/// scalar assignment operators
	template <unsigned n, typename Iterator, typename T>
		Slice<n, Iterator> &operator +=(Slice<n, Iterator> &, T);

	template <unsigned n, typename Iterator, typename T>
		Slice<n, Iterator> &operator -=(Slice<n, Iterator> &, T);

	template <unsigned n, typename Iterator, typename T>
		Slice<n, Iterator> &operator *=(Slice<n, Iterator> &, T);

	template <unsigned n, typename Iterator, typename T>
		Slice<n, Iterator> &operator /=(Slice<n, Iterator> &, T);

	template <unsigned n, typename Iterator, typename T>
		Slice<n, Iterator> &operator %=(Slice<n, Iterator> &, T);

	/// stream insertion & extraction
	template <typename Char, typename CharTraits, unsigned n, typename Iterator>
		std::basic_ostream<Char, CharTraits> &operator <<(std::basic_ostream<Char, CharTraits> &, const Slice<n, Iterator> &);

	template <typename Char, typename CharTraits, unsigned n, typename Iterator>
		std::basic_istream<Char, CharTraits> &operator >>(std::basic_istream<Char, CharTraits> &, const Slice<n, Iterator> &);
}}

#	include "Slice.tpp"
#endif
