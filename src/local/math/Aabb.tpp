#include <algorithm> // swap

namespace page { namespace math
{
	/*-------------+
	| constructors |
	+-------------*/

	template <unsigned n, typename T> Aabb<n, T>::Aabb(T t) :
		min(t), max(t) {}
	template <unsigned n, typename T> Aabb<n, T>::Aabb(const Vector<n, T> &v) :
		min(v), max(v) {}
	template <unsigned n, typename T> Aabb<n, T>::Aabb(T min, T max) :
		min(min), max(max) {}
	template <unsigned n, typename T> Aabb<n, T>::Aabb(const Vector<n, T> &min, const Vector<n, T> &max) :
		min(min), max(max) {}
	template <unsigned n, typename T> template <unsigned m, typename U> Aabb<n, T>::Aabb(const Aabb<m, U> &other) :
		min(other.min), max(other.max) {}

	/*-----------+
	| conversion |
	+-----------*/

	template <unsigned n, typename T> template <typename U> Aabb<n, T>::operator Aabb<n, U>() const
	{
		return Aabb<n, U>(min, max);
	}

	/*---------------+
	| initialization |
	+---------------*/

	template <unsigned n, typename T, typename U> Aabb<n, typename ArithmeticConversion<T, U>::Result> AabbPositionSize(const Vector<n, T> &position, const Vector<n, U> &size)
	{
		typedef typename ArithmeticConversion<T, U>::Result R;
		return Aabb<n, R>(position, position + size);
	}
	template <unsigned n, typename T> Aabb<n, T> InverseInfiniteAabb()
	{
		return Aabb<n, T>(Inf<T>(), -Inf<T>());
	}
	template <unsigned n, typename T> Aabb<n, T> UnitAabb()
	{
		return Aabb<n, T>(0, 1);
	}

	/*----------------+
	| unary operators |
	+----------------*/

	template <unsigned n, typename T> Aabb<n, T> operator +(const Aabb<n, T> &a)
	{
		return a;
	}
	template <unsigned n, typename T> Aabb<n, T> operator -(const Aabb<n, T> &a)
	{
		return Aabb<n, T>(-a.min, -a.max);
	}

	/*----------------------------+
	| vector arithmetic operators |
	+----------------------------*/

	template <unsigned n, typename T, typename U> Aabb<n, typename ArithmeticConversion<T, U>::Result> operator *(const Aabb<n, T> &a, const Vector<n, U> &v)
	{
		typedef typename ArithmeticConversion<T, U>::Result R;
		return Aabb<n, R>(a.min * v, a.max * v);
	}
	template <unsigned n, typename T, typename U> Aabb<n, typename ArithmeticConversion<T, U>::Result> operator /(const Aabb<n, T> &a, const Vector<n, U> &v)
	{
		typedef typename ArithmeticConversion<T, U>::Result R;
		return Aabb<n, R>(a.min / v, a.max / v);
	}
	template <unsigned n, typename T, typename U> Aabb<n, typename ArithmeticConversion<T, U>::Result> operator +(const Aabb<n, T> &a, const Vector<n, U> &v)
	{
		typedef typename ArithmeticConversion<T, U>::Result R;
		return Aabb<n, R>(a.min + v, a.max + v);
	}
	template <unsigned n, typename T, typename U> Aabb<n, typename ArithmeticConversion<T, U>::Result> operator -(const Aabb<n, T> &a, const Vector<n, U> &v)
	{
		typedef typename ArithmeticConversion<T, U>::Result R;
		return Aabb<n, R>(a.min - v, a.max - v);
	}

	/*------------------------------------+
	| reverse vector arithmetic operators |
	+------------------------------------*/

	template <unsigned n, typename T, typename U> Aabb<n, typename ArithmeticConversion<T, U>::Result> operator *(const Vector<n, T> &v, const Aabb<n, U> &a)
	{
		typedef typename ArithmeticConversion<T, U>::Result R;
		return Aabb<n, R>(v * a.min, v * a.max);
	}
	template <unsigned n, typename T, typename U> Aabb<n, typename ArithmeticConversion<T, U>::Result> operator /(const Vector<n, T> &v, const Aabb<n, U> &a)
	{
		typedef typename ArithmeticConversion<T, U>::Result R;
		return Aabb<n, R>(v / a.min, v / a.max);
	}
	template <unsigned n, typename T, typename U> Aabb<n, typename ArithmeticConversion<T, U>::Result> operator +(const Vector<n, T> &v, const Aabb<n, U> &a)
	{
		typedef typename ArithmeticConversion<T, U>::Result R;
		return Aabb<n, R>(v + a.min, v + a.max);
	}
	template <unsigned n, typename T, typename U> Aabb<n, typename ArithmeticConversion<T, U>::Result> operator -(const Vector<n, T> &v, const Aabb<n, U> &a)
	{
		typedef typename ArithmeticConversion<T, U>::Result R;
		return Aabb<n, R>(v - a.min, v - a.max);
	}

	/*----------------------------+
	| scalar arithmetic operators |
	+----------------------------*/

	template <unsigned n, typename T, typename U> Aabb<n, typename ArithmeticConversion<T, U>::Result> operator *(const Aabb<n, T> &a, U t)
	{
		typedef typename ArithmeticConversion<T, U>::Result R;
		return Aabb<n, R>(a.min * t, a.max * t);
	}
	template <unsigned n, typename T, typename U> Aabb<n, typename ArithmeticConversion<T, U>::Result> operator /(const Aabb<n, T> &a, U t)
	{
		typedef typename ArithmeticConversion<T, U>::Result R;
		return Aabb<n, R>(a.min / t, a.max / t);
	}
	template <unsigned n, typename T, typename U> Aabb<n, typename ArithmeticConversion<T, U>::Result> operator +(const Aabb<n, T> &a, U t)
	{
		typedef typename ArithmeticConversion<T, U>::Result R;
		return Aabb<n, R>(a.min + t, a.max + t);
	}
	template <unsigned n, typename T, typename U> Aabb<n, typename ArithmeticConversion<T, U>::Result> operator -(const Aabb<n, T> &a, U t)
	{
		typedef typename ArithmeticConversion<T, U>::Result R;
		return Aabb<n, R>(a.min - t, a.max - t);
	}

	/*------------------------------------+
	| reverse scalar arithmetic operators |
	+------------------------------------*/

	template <unsigned n, typename T, typename U> Aabb<n, typename ArithmeticConversion<T, U>::Result> operator *(T t, const Aabb<n, U> &a)
	{
		typedef typename ArithmeticConversion<T, U>::Result R;
		return Aabb<n, R>(t * a.min, t * a.max);
	}
	template <unsigned n, typename T, typename U> Aabb<n, typename ArithmeticConversion<T, U>::Result> operator /(T t, const Aabb<n, U> &a)
	{
		typedef typename ArithmeticConversion<T, U>::Result R;
		return Aabb<n, R>(t / a.min, t / a.max);
	}
	template <unsigned n, typename T, typename U> Aabb<n, typename ArithmeticConversion<T, U>::Result> operator +(T t, const Aabb<n, U> &a)
	{
		typedef typename ArithmeticConversion<T, U>::Result R;
		return Aabb<n, R>(t + a.min, t + a.max);
	}
	template <unsigned n, typename T, typename U> Aabb<n, typename ArithmeticConversion<T, U>::Result> operator -(T t, const Aabb<n, U> &a)
	{
		typedef typename ArithmeticConversion<T, U>::Result R;
		return Aabb<n, R>(t - a.min, t - a.max);
	}

	/*---------------------+
	| relational operators |
	+---------------------*/

	template <unsigned n, typename T, typename U> bool operator ==(const Aabb<n, T> &a1, const Aabb<n, U> &a2)
	{
		return All(a1.min == a2.min && a1.max == a2.max);
	}
	template <unsigned n, typename T, typename U> bool operator !=(const Aabb<n, T> &a1, const Aabb<n, U> &a2)
	{
		return Any(a1.min != a2.min || a1.max != a2.max);
	}

	/*----------------------------+
	| vector assignment operators |
	+----------------------------*/

	template <unsigned n, typename T, typename U> Aabb<n, T> &operator *=(Aabb<n, T> &a, const Vector<n, U> &v)
	{
		a.min *= v;
		a.max *= v;
		return a;
	}
	template <unsigned n, typename T, typename U> Aabb<n, T> &operator /=(Aabb<n, T> &a, const Vector<n, U> &v)
	{
		a.min /= v;
		a.max /= v;
		return a;
	}
	template <unsigned n, typename T, typename U> Aabb<n, T> &operator +=(Aabb<n, T> &a, const Vector<n, U> &v)
	{
		a.min += v;
		a.max += v;
		return a;
	}
	template <unsigned n, typename T, typename U> Aabb<n, T> &operator -=(Aabb<n, T> &a, const Vector<n, U> &v)
	{
		a.min -= v;
		a.max -= v;
		return a;
	}

	/*----------------------------+
	| scalar assignment operators |
	+----------------------------*/

	template <unsigned n, typename T, typename U> Aabb<n, T> &operator *=(Aabb<n, T> &a, U t)
	{
		a.min *= t;
		a.max *= t;
		return a;
	}
	template <unsigned n, typename T, typename U> Aabb<n, T> &operator /=(Aabb<n, T> &a, U t)
	{
		a.min /= t;
		a.max /= t;
		return a;
	}
	template <unsigned n, typename T, typename U> Aabb<n, T> &operator +=(Aabb<n, T> &a, U t)
	{
		a.min -= t;
		a.max -= t;
		return a;
	}
	template <unsigned n, typename T, typename U> Aabb<n, T> &operator -=(Aabb<n, T> &a, U t)
	{
		a.min += t;
		a.max += t;
		return a;
	}

	/*--------+
	| min/max |
	+--------*/

	template <unsigned n, typename T, typename U> Aabb<n, typename ArithmeticConversion<T, U>::Result> Min(const Aabb<n, T> &a1, const Aabb<n, U> &a2)
	{
		typedef typename ArithmeticConversion<T, U>::Result R;
		return Aabb<n, R>(
			Max(Min(a1.min, a2.max), a2.min),
			Min(Max(a1.max, a2.min), a2.max));
	}
	template <unsigned n, typename T, typename U> Aabb<n, typename ArithmeticConversion<T, U>::Result> Max(const Aabb<n, T> &a1, const Aabb<n, U> &a2)
	{
		typedef typename ArithmeticConversion<T, U>::Result R;
		return Aabb<n, R>(Min(a1.min, a2.min), Max(a1.max, a2.max));
	}
	template <unsigned n, typename T, typename U> Aabb<n, typename ArithmeticConversion<T, U>::Result> Max(const Aabb<n, T> &a, const Vector<n, U> &v)
	{
		return Max(a, Aabb<n, U>(v));
	}
	template <unsigned n, typename T, typename U> Aabb<n, typename ArithmeticConversion<T, U>::Result> Max(const Vector<n, T> &v, const Aabb<n, U> &a)
	{
		return Max(Aabb<n, T>(v), a);
	}

	/*------------+
	| grow/shrink |
	+------------*/

	template <unsigned n, typename T, typename U> Aabb<n, typename ArithmeticConversion<T, U>::Result> Grow(const Aabb<n, T> &a, U t)
	{
		typedef typename ArithmeticConversion<T, U>::Result R;
		return Aabb<n, R>(a.min - t, a.max + t);
	}
	template <unsigned n, typename T, typename U> Aabb<n, typename ArithmeticConversion<T, U>::Result> Grow(const Aabb<n, T> &a, const Vector<n, U> &v)
	{
		typedef typename ArithmeticConversion<T, U>::Result R;
		return Aabb<n, R>(a.min - v, a.max + v);
	}
	template <unsigned n, typename T, typename U> Aabb<n, typename ArithmeticConversion<T, U>::Result> Grow(const Aabb<n, T> &a, U min, U max)
	{
		typedef typename ArithmeticConversion<T, U>::Result R;
		return Aabb<n, R>(a.min - min, a.max + max);
	}
	template <unsigned n, typename T, typename U> Aabb<n, typename ArithmeticConversion<T, U>::Result> Grow(const Aabb<n, T> &a, const Vector<n, U> &min, const Vector<n, U> &max)
	{
		typedef typename ArithmeticConversion<T, U>::Result R;
		return Aabb<n, R>(a.min - min, a.max + max);
	}
	template <unsigned n, typename T, typename U> Aabb<n, typename ArithmeticConversion<T, U>::Result> Shrink(const Aabb<n, T> &a, U t)
	{
		typedef typename ArithmeticConversion<T, U>::Result R;
		return Aabb<n, R>(a.min + t, a.max - t);
	}
	template <unsigned n, typename T, typename U> Aabb<n, typename ArithmeticConversion<T, U>::Result> Shrink(const Aabb<n, T> &a, const Vector<n, U> &v)
	{
		typedef typename ArithmeticConversion<T, U>::Result R;
		return Aabb<n, R>(a.min + v, a.max - v);
	}
	template <unsigned n, typename T, typename U> Aabb<n, typename ArithmeticConversion<T, U>::Result> Shrink(const Aabb<n, T> &a, U min, U max)
	{
		typedef typename ArithmeticConversion<T, U>::Result R;
		return Aabb<n, R>(a.min + min, a.max - max);
	}
	template <unsigned n, typename T, typename U> Aabb<n, typename ArithmeticConversion<T, U>::Result> Shrink(const Aabb<n, T> &a, const Vector<n, U> &min, const Vector<n, U> &max)
	{
		typedef typename ArithmeticConversion<T, U>::Result R;
		return Aabb<n, R>(a.min + min, a.max - max);
	}

	/*------------+
	| containment |
	+------------*/

	template <unsigned n, typename T, typename U> bool Contains(const Aabb<n, T> &container, const Aabb<n, U> &a)
	{
		return All(a.min >= container.min && a.max <= container.max);
	}
	template <unsigned n, typename T, typename U> bool Contains(const Aabb<n, T> &container, const Vector<n, U> &v)
	{
		return All(v >= container.min && v <= container.max);
	}
	template <unsigned n, typename T, typename U> bool ContainsExclusive(const Aabb<n, T> &container, const Aabb<n, U> &a)
	{
		return All(a.min > container.min && a.max < container.max);
	}
	template <unsigned n, typename T, typename U> bool ContainsExclusive(const Aabb<n, T> &container, const Vector<n, U> &v)
	{
		return All(v > container.min && v < container.max);
	}
	template <unsigned n, typename T, typename U> bool ContainsMinExclusive(const Aabb<n, T> &container, const Aabb<n, U> &a)
	{
		return All(a.min > container.min && a.max <= container.max);
	}
	template <unsigned n, typename T, typename U> bool ContainsMinExclusive(const Aabb<n, T> &container, const Vector<n, U> &v)
	{
		return All(v > container.min && v <= container.max);
	}
	template <unsigned n, typename T, typename U> bool ContainsMaxExclusive(const Aabb<n, T> &container, const Aabb<n, U> &a)
	{
		return All(a.min >= container.min && a.max < container.max);
	}
	template <unsigned n, typename T, typename U> bool ContainsMaxExclusive(const Aabb<n, T> &container, const Vector<n, U> &v)
	{
		return All(v >= container.min && v < container.max);
	}

	/*---------------------------------+
	| spatial reference transformation |
	+---------------------------------*/

	template <unsigned n, typename T, typename U> Aabb<n, typename Real<typename ArithmeticConversion<T, U>::Result>::Type> EnterSpace(const Aabb<n, T> &space, const Aabb<n, U> &a)
	{
		typedef typename Real<typename ArithmeticConversion<T, U>::Result>::Type R;
		return Aabb<n, R>(
			EnterSpace(space, a.min),
			EnterSpace(space, a.max));
	}
	template <unsigned n, typename T, typename U> Vector<n, typename Real<typename ArithmeticConversion<T, U>::Result>::Type> EnterSpace(const Aabb<n, T> &space, const Vector<n, U> &v)
	{
		typedef typename Real<typename ArithmeticConversion<T, U>::Result>::Type R;
		return Vector<n, R>(v - space.min) / Size(space);
	}
	template <unsigned n, typename T, typename U> Aabb<n, typename ArithmeticConversion<T, U>::Result> LeaveSpace(const Aabb<n, T> &space, const Aabb<n, U> &a)
	{
		typedef typename ArithmeticConversion<T, U>::Result R;
		return Aabb<n, R>(
			LeaveSpace(space, a.min),
			LeaveSpace(space, a.max));
	}
	template <unsigned n, typename T, typename U> Vector<n, typename ArithmeticConversion<T, U>::Result> LeaveSpace(const Aabb<n, T> &space, const Vector<n, U> &v)
	{
		return space.min + v * Size(space);
	}

	/*-----------------+
	| other operations |
	+-----------------*/

	template <typename T> typename Real<T>::Type Aspect(const Aabb<2, T> &a)
	{
		typedef typename Real<T>::Type R;
		return R(a.max.x - a.min.x) / (a.max.y - a.min.y);
	}
	template <unsigned n, typename T> Aabb<n, T> Ceil(const Aabb<n, T> &a)
	{
		return Aabb<n, T>(Floor(a.min), Ceil(a.max));
	}
	template <unsigned n, typename T> Vector<n, T> Center(const Aabb<n, T> &a)
	{
		return a.min + Size(a) / 2;
	}
	template <unsigned n, typename T, typename U> Vector<n, typename ArithmeticConversion<T, U>::Result> Center(const Aabb<n, T> &a, const Vector<n, U> &v)
	{
		return a.min + Size(a) * v;
	}
	template <unsigned n, typename T> Aabb<n, T> Floor(const Aabb<n, T> &a)
	{
		return Aabb<n, T>(Ceil(a.min), Floor(a.max));
	}
	template <unsigned n, typename T> Vector<n, T> Size(const Aabb<n, T> &a)
	{
		return a.max - a.min;
	}

	/*----------------------------+
	| stream insertion/extraction |
	+----------------------------*/

	template <typename Char, typename CharTraits, unsigned n, typename T>
		std::basic_ostream<Char, CharTraits> &operator <<(std::basic_ostream<Char, CharTraits> &os, const Aabb<n, T> &a)
	{
		return os <<
			CharTraits::to_char_type('(') << a.min;
			CharTraits::to_char_type(',') << a.max;
			CharTraits::to_char_type(')');
	}
	template <typename Char, typename CharTraits, unsigned n, typename T>
		std::basic_istream<Char, CharTraits> &operator >>(std::basic_istream<Char, CharTraits> &is, Aabb<n, T> &a)
	{
		// read opening bracket
		Char ch;
		is >> ch;
		if (!CharTraits::eq(ch, CharTraits::to_char_type('(')))
			is.setstate(std::ios_base::failbit);

		is >> a.min;

		// read separator
		is >> ch;
		if (!CharTraits::eq(ch, CharTraits::to_char_type(',')))
			is.setstate(std::ios_base::failbit);

		is >> a.max;

		// read closing bracket
		is >> ch;
		if (!CharTraits::eq(ch, CharTraits::to_char_type(')')))
			is.setstate(std::ios_base::failbit);

		return is;
	}

	/*-------------------------------+
	| standard-library compatibility |
	+-------------------------------*/

	template <unsigned n, typename T> void swap(Aabb<n, T> &a, Aabb<n, T> &b)
	{
		using std::swap;
		swap(a.min, b.min);
		swap(a.max, b.max);
	}
}}
