#ifndef    page_local_math_win32_hpp
#   define page_local_math_win32_hpp

#	include <windows.h> // LONG, POINT, RECT

#	include "Aabb.hpp"
#	include "Vector.hpp"

namespace page { namespace math { namespace win32
{
	/// from Win32
	Aabb<2, LONG> MakeAabb(const RECT &);
	Vector<2, LONG> MakeVector(const POINT &);
	Vec2i MakeVector(const LPARAM &);

	/// to Win32
	POINT MakePoint(const Vector<2, LONG> &);
	RECT MakeRect(const Aabb<2, LONG> &);
}}}

#endif
