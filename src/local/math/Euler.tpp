#include <algorithm> // swap
#include <cmath> // a{sin,tan2}, cos, sin

#include "Axan.hpp"
#include "float.hpp" // DegTo{Rad,Rev}, Pi, RadTo{Deg,Rev}, RevTo{Deg,Rad}
#include "Matrix.hpp"
#include "Quat.hpp"

namespace page { namespace math
{
	// constructors
	template <typename T> Euler<T>::Euler(T yaw, T pitch, T roll) :
		yaw(yaw), pitch(pitch), roll(roll) {}
	template <typename T> template <typename U> Euler<T>::Euler(const Euler<U> &other) :
		yaw(other.yaw), pitch(other.pitch), roll(other.roll) {}
	template <typename T> Euler<T>::Euler(const Axan<T> &a)
	{
		T
			s = std::sin(a.angle),
			c = std::cos(a.angle), ic = 1 - c,
			test = a.axis.x * s - a.axis.y * a.axis.z * ic;
		if (test > .999f) // north pole singularity
		{
			yaw   = 0;
			pitch = 0;
			roll  = 0;
			return;
		}
		if (test < -.999f) // south pole singularity
		{
			yaw   = 0;
			pitch = 0;
			roll  = 0;
			return;
		}
		yaw   = std::atan2(a.axis.x * a.axis.z * ic + a.axis.y * s, 1 - (a.axis.x * a.axis.x + a.axis.y * a.axis.y) * ic);
		pitch = std::asin(test);
		roll  = std::atan2(a.axis.x * a.axis.y * ic + a.axis.z * s, 1 - (a.axis.x * a.axis.x + a.axis.z * a.axis.z) * ic);
	}
	template <typename T> Euler<T>::Euler(const Matrix<3, 3, T> &m)
	{
		if (m.yz > .999f) // north pole singularity
		{
			yaw   = std::atan2(m.xy, m.zy);
			pitch = Pi<T>() / 2;
			roll  = 0;
			return;
		}
		if (m.yz < -.999f) // south pole singularity
		{
			yaw   = std::atan2(m.xy, m.zy);
			pitch = -Pi<T>() / 2;
			roll  = 0;
			return;
		}
		yaw   = std::atan2(m.xz, m.zz);
		pitch = std::asin(-m.yz);
		roll  = std::atan2(m.yx, m.yy);
	}
	template <typename T> Euler<T>::Euler(const Quat<T> &q)
	{
		assert(Near(q, Norm(q)));
		T test = q.w * q.x - q.y * q.z;
		if (test > .4995f) // north pole singularity
		{
			yaw   = std::atan2(q.y, q.w) * 2;
			pitch = Pi<T>() / 2;
			roll  = 0;
			return;
		}
		if (test < -.4995f) // south pole singularity
		{
			yaw   = -std::atan2(q.y, q.w) * 2;
			pitch = -Pi<T>() / 2;
			roll  = 0;
			return;
		}
		yaw   = std::atan2((q.x * q.z + q.w * q.y) * 2, 1 - (q.x * q.x + q.y * q.y) * 2);
		pitch = std::asin(test * 2);
		roll  = std::atan2((q.x * q.y + q.w * q.z) * 2, 1 - (q.x * q.x + q.z * q.z) * 2);
	}
	template <typename T> Euler<T>::Euler(const Vector<3, T> &v) :
		yaw(v.x), pitch(v.y), roll(v.z) {}
	template <typename T> Euler<T>::Euler(const Vector<3, T> &v1, const Vector<3, T> &v2) :
		roll(0)
	{
		// FIXME: unstable with vertical vectors
		yaw = AngleBetween(Swizzle(v1, 0, 2), Swizzle(v2, 0, 2));
		// FIXME: pitch must be negated to make
		// res::CameraSet::Camera::orientationTracking work; why?
		pitch = -AngleBetween(Axan<T>(0, 1, 0, yaw) * v1, v2);
	}

	// conversion
	template <typename T> template <typename U> Euler<T>::operator Euler<U>() const
	{
		Euler<U> r;
		r.yaw   = yaw;
		r.pitch = pitch;
		r.roll  = roll;
		return r;
	}
	template <typename T> Euler<T>::operator Vector<3, T>() const
	{
		return Vector<3, T>(yaw, pitch, roll);
	}

	// arithmetic operators
	template <typename T, typename U> Euler<typename ArithmeticConversion<T, U>::Result> operator +(const Euler<T> &e1, const Euler<U> &e2)
	{
		typedef typename ArithmeticConversion<T, U>::Result R;
		return Euler<R>(
			e1.yaw   + e2.yaw,
			e1.pitch + e2.pitch,
			e1.roll  + e2.roll);
	}
	template <typename T, typename U> Euler<typename ArithmeticConversion<T, U>::Result> operator -(const Euler<T> &e1, const Euler<U> &e2)
	{
		typedef typename ArithmeticConversion<T, U>::Result R;
		return Euler<R>(
			e1.yaw   - e2.yaw,
			e1.pitch - e2.pitch,
			e1.roll  - e2.roll);
	}

	// scalar arithmetic operators
	template <typename T, typename U> Euler<typename ArithmeticConversion<T, U>::Result> operator *(const Euler<T> &e, U t)
	{
		typedef typename ArithmeticConversion<T, U>::Result R;
		return Euler<R>(
			e.yaw   * t,
			e.pitch * t,
			e.roll  * t);
	}
	template <typename T, typename U> Euler<typename ArithmeticConversion<T, U>::Result> operator /(const Euler<T> &e, U t)
	{
		typedef typename ArithmeticConversion<T, U>::Result R;
		return Euler<R>(
			e.yaw   / t,
			e.pitch / t,
			e.roll  / t);
	}
	template <typename T, typename U> Euler<typename ArithmeticConversion<T, U>::Result> operator +(const Euler<T> &e, U t)
	{
		typedef typename ArithmeticConversion<T, U>::Result R;
		return Euler<R>(
			e.yaw   + t,
			e.pitch + t,
			e.roll  + t);
	}
	template <typename T, typename U> Euler<typename ArithmeticConversion<T, U>::Result> operator -(const Euler<T> &e, U t)
	{
		typedef typename ArithmeticConversion<T, U>::Result R;
		return Euler<R>(
			e.yaw   - t,
			e.pitch - t,
			e.roll  - t);
	}

	// relational operators
	template <typename T, typename U> Vector<3, bool> operator <(const Euler<T> &e1, const Euler<U> &e2)
	{
		return Vector<3, bool>(
			e1.yaw   < e2.yaw,
			e1.pitch < e2.pitch,
			e1.roll  < e2.roll);
	}
	template <typename T, typename U> Vector<3, bool> operator >(const Euler<T> &e1, const Euler<U> &e2)
	{
		return Vector<3, bool>(
			e1.yaw   > e2.yaw,
			e1.pitch > e2.pitch,
			e1.roll  > e2.roll);
	}
	template <typename T, typename U> Vector<3, bool> operator <=(const Euler<T> &e1, const Euler<U> &e2)
	{
		return Vector<3, bool>(
			e1.yaw   <= e2.yaw,
			e1.pitch <= e2.pitch,
			e1.roll  <= e2.roll);
	}
	template <typename T, typename U> Vector<3, bool> operator >=(const Euler<T> &e1, const Euler<U> &e2)
	{
		return Vector<3, bool>(
			e1.yaw   >= e2.yaw,
			e1.pitch >= e2.pitch,
			e1.roll  >= e2.roll);
	}
	template <typename T, typename U> Vector<3, bool> operator ==(const Euler<T> &e1, const Euler<U> &e2)
	{
		return Vector<3, bool>(
			e1.yaw   == e2.yaw,
			e1.pitch == e2.pitch,
			e1.roll  == e2.roll);
	}
	template <typename T, typename U> Vector<3, bool> operator !=(const Euler<T> &e1, const Euler<U> &e2)
	{
		return Vector<3, bool>(
			e1.yaw   != e2.yaw,
			e1.pitch != e2.pitch,
			e1.roll  != e2.roll);
	}

	// assignment operators
	template <typename T, typename U> Euler<T> &operator +=(Euler<T> &e1, const Euler<U> &e2)
	{
		e1.yaw   += e2.yaw;
		e1.pitch += e2.pitch;
		e1.roll  += e2.roll;
		return e1;
	}
	template <typename T, typename U> Euler<T> &operator -=(Euler<T> &e1, const Euler<U> &e2)
	{
		e1.yaw   -= e2.yaw;
		e1.pitch -= e2.pitch;
		e1.roll  -= e2.roll;
		return e1;
	}

	// scalar assignment operators
	template <typename T, typename U> Euler<T> &operator *=(Euler<T> &e, U t)
	{
		e.yaw   *= t;
		e.pitch *= t;
		e.roll  *= t;
		return e;
	}
	template <typename T, typename U> Euler<T> &operator /=(Euler<T> &e, U t)
	{
		e.yaw   /= t;
		e.pitch /= t;
		e.roll  /= t;
		return e;
	}
	template <typename T, typename U> Euler<T> &operator +=(Euler<T> &e, U t)
	{
		e.yaw   += t;
		e.pitch += t;
		e.roll  += t;
		return e;
	}
	template <typename T, typename U> Euler<T> &operator -=(Euler<T> &e, U t)
	{
		e.yaw   -= t;
		e.pitch -= t;
		e.roll  -= t;
		return e;
	}

	// angle conversions
	template <typename T> Euler<T> DegToRad(const Euler<T> &e)
	{
		return Euler<T>(
			DegToRad(e.yaw),
			DegToRad(e.pitch),
			DegToRad(e.roll));
	}
	template <typename T> Euler<T> DegToRev(const Euler<T> &e)
	{
		return Euler<T>(
			DegToRev(e.yaw),
			DegToRev(e.pitch),
			DegToRev(e.roll));
	}
	template <typename T> Euler<T> RadToDeg(const Euler<T> &e)
	{
		return Euler<T>(
			RadToDeg(e.yaw),
			RadToDeg(e.pitch),
			RadToDeg(e.roll));
	}
	template <typename T> Euler<T> RadToRev(const Euler<T> &e)
	{
		return Euler<T>(
			RadToRev(e.yaw),
			RadToRev(e.pitch),
			RadToRev(e.roll));
	}
	template <typename T> Euler<T> RevToDeg(const Euler<T> &e)
	{
		return Euler<T>(
			RevToDeg(e.yaw),
			RevToDeg(e.pitch),
			RevToDeg(e.roll));
	}
	template <typename T> Euler<T> RevToRad(const Euler<T> &e)
	{
		return Euler<T>(
			RevToRad(e.yaw),
			RevToRad(e.pitch),
			RevToRad(e.roll));
	}

	// vector transformation
	template <typename T, typename U> Vector<3, typename ArithmeticConversion<T, U>::Result> operator *(const Euler<T> &e, const Vector<3, U> &v)
	{
		return Quat<T>(e) * v;
	}

	// stream insertion/extraction
	template <typename Char, typename CharTraits, typename T>
		std::basic_ostream<Char, CharTraits> &operator <<(std::basic_ostream<Char, CharTraits> &os, const Euler<T> &e)
	{
		return os <<
			CharTraits::to_char_type('(') << e.yaw   <<
			CharTraits::to_char_type(',') << e.pitch <<
			CharTraits::to_char_type(',') << e.roll  <<
			CharTraits::to_char_type(')');
	}
	template <typename Char, typename CharTraits, typename T>
		std::basic_istream<Char, CharTraits> &operator >>(std::basic_istream<Char, CharTraits> &is, Euler<T> &e)
	{
		// read opening bracket
		Char ch;
		is >> ch;
		if (!CharTraits::eq(ch, CharTraits::to_char_type('(')))
			is.setstate(std::ios_base::failbit);

		is >> e.yaw;

		// read separator
		is >> ch;
		if (!CharTraits::eq(ch, CharTraits::to_char_type(',')))
			is.setstate(std::ios_base::failbit);

		is >> e.pitch;

		// read separator
		is >> ch;
		if (!CharTraits::eq(ch, CharTraits::to_char_type(',')))
			is.setstate(std::ios_base::failbit);

		is >> e.roll;

		// read closing bracket
		is >> ch;
		if (!CharTraits::eq(ch, CharTraits::to_char_type(')')))
			is.setstate(std::ios_base::failbit);

		return is;
	}

	// standard-library compatibility
	template <typename T> void swap(Euler<T> &a, Euler<T> &b)
	{
		using std::swap;
		swap(a.yaw,   b.yaw);
		swap(a.pitch, b.pitch);
		swap(a.roll,  b.roll);
	}
}}
