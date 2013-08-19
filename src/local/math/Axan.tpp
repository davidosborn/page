#include <algorithm> // max, min, swap
#include <cassert>
#include <cmath> // acos, cos, sin, sqrt

#include "Euler.hpp"
#include "float.hpp" // Near
#include "Matrix.hpp"
#include "Quat.hpp"
#include "Vector.hpp"

namespace page { namespace math
{
	// constructors
	template <typename T> Axan<T>::Axan(T x, T y, T z, T angle) :
		axis(x, y, z), angle(angle) {}
	template <typename T> Axan<T>::Axan(const Vector<3, T> &axis, T angle) :
		axis(axis), angle(angle) {}
	template <typename T> template <typename U> Axan<T>::Axan(const Axan<U> &other) :
		axis(other.axis), angle(other.angle) {}
	template <typename T> Axan<T>::Axan(const Euler<T> &e)
	{
		T
			cy = std::cos(e.yaw   / 2), sy = std::sin(e.yaw   / 2),
			cp = std::cos(e.pitch / 2), sp = std::sin(e.pitch / 2),
			cr = std::cos(e.roll  / 2), sr = std::sin(e.roll  / 2);
		axis = Vector<3, T>(
			cy * sp * cr + sy * cp * sr,
			sy * cp * cr - cy * sp * sr,
			cy * cp * sr - sy * sp * cr);
		T norm = Dot(axis, axis);
		if (norm < .001f) axis = NormVector<3, T>();
		else axis /= std::sqrt(norm);
		angle = std::acos(cy * cp * cr + sy * sp * sr) * 2;
	}
	template <typename T> Axan<T>::Axan(const Matrix<3, 3, T> &m) :
		axis(Norm(Vector<3, T>(m.zy - m.yz, m.xz - m.zx, m.yx - m.xy))),
		angle(std::acos(std::min<T>(std::max<T>((m.xx + m.yy + m.zz - 1) / 2, -1), 1))) {}
	template <typename T> Axan<T>::Axan(const Quat<T> &q)
	{
		assert(Near(q, Norm(q)));
		T len = std::sqrt(1 - q.w * q.w);
		axis = Near(len, 0) ? NormVector<3, T>() : Vector<3, T>(q) / len;
		angle = std::min<T>(std::max<T>(std::acos(q.w), -1), 1) * 2;
	}
	template <typename T> Axan<T>::Axan(const Vector<4, T> &v) :
		axis(v), angle(v.w) {}
	template <typename T> Axan<T>::Axan(const Vector<3, T> &v1, const Vector<3, T> &v2)
	{
		assert(All(Near(v1, Norm(v1))));
		assert(All(Near(v2, Norm(v2))));
		T t = Dot(v1, v2);
		axis =
			Near(t, -1) ? Swizzle(v1, 2, 1, 0) :
			Near(t, 1) ? NormVector<3, T>() : Norm(Cross(v1, v2));
		angle = std::acos(std::min<T>(std::max<T>(t, -1), 1));
	}

	// conversion
	template <typename T> template <typename U> Axan<T>::operator Axan<U>() const
	{
		return Axan<U>(axis, angle);
	}

	// relational operators
	template <typename T, typename U> bool operator ==(const Axan<T> &a1, const Axan<U> &a2)
	{
		return All(a1.axis == a2.axis) && a1.angle == a2.angle;
	}
	template <typename T, typename U> bool operator !=(const Axan<T> &a1, const Axan<U> &a2)
	{
		return Any(a1.axis != a2.axis) || a1.angle != a2.angle;
	}

	// other operations
	template <typename T, typename U> Axan<typename ArithmeticConversion<T, U>::Result> Between(const Axan<T> &a1, const Axan<U> &a2)
	{
		typedef typename ArithmeticConversion<T, U>::Result R;
		return Axan<R>(a1 * NormVector<3, T>(), a2 * NormVector<3, U>());
	}
	template <typename T, typename U> bool Near(const Axan<T> &a1, const Axan<U> &a2)
	{
		return Near(a1.axis, a2.axis) && Near(a1.angle, a2.angle);
	}

	// vector transformation
	template <typename T, typename U> Vector<3, typename ArithmeticConversion<T, U>::Result> operator *(const Axan<T> &a, const Vector<3, U> &v)
	{
		typedef typename ArithmeticConversion<T, U>::Result R;
		R
			s = std::sin(a.angle),
			c = std::cos(a.angle);
		return v + (1 - c) * Cross(a.axis, Cross(a.axis, v)) + s * Cross(a.axis, v);
	}

	// stream insertion/extraction
	template <typename Char, typename CharTraits, typename T>
		std::basic_ostream<Char, CharTraits> &operator <<(std::basic_ostream<Char, CharTraits> &os, const Axan<T> &a)
	{
		return os <<
			CharTraits::to_char_type('(') << a.axis;
			CharTraits::to_char_type(',') << a.angle;
			CharTraits::to_char_type(')');
	}
	template <typename Char, typename CharTraits, typename T>
		std::basic_istream<Char, CharTraits> &operator >>(std::basic_istream<Char, CharTraits> &is, Axan<T> &a)
	{
		// read opening bracket
		Char ch;
		is >> ch;
		if (!CharTraits::eq(ch, CharTraits::to_char_type('(')))
			is.setstate(std::ios_base::failbit);

		is >> a.axis;

		// read separator
		is >> ch;
		if (!CharTraits::eq(ch, CharTraits::to_char_type(',')))
			is.setstate(std::ios_base::failbit);

		is >> a.angle;

		// read closing bracket
		is >> ch;
		if (!CharTraits::eq(ch, CharTraits::to_char_type(')')))
			is.setstate(std::ios_base::failbit);

		return is;
	}

	// standard-library compatibility
	template <typename T> void swap(Axan<T> &a, Axan<T> &b)
	{
		using std::swap;
		swap(a.axis,  b.axis);
		swap(a.angle, b.angle);
	}
}}
