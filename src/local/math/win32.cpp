#include <windows.h> // LONG, POINT, RECT
#include <windowsx.h> // GET_[XY]_LPARAM

#include "Aabb.hpp"
#include "Vector.hpp"

namespace page { namespace math { namespace win32
{
	/// from Win32
	Aabb<2, LONG> MakeAabb(const RECT &rect)
	{
		return Aabb<2, LONG>(
			Vector<2, LONG>(rect.left, rect.top),
			Vector<2, LONG>(rect.right, rect.bottom));
	}

	Vector<2, LONG> MakeVector(const POINT &pt)
	{
		return Vector<2, LONG>(pt.x, pt.y);
	}

	Vec2i MakeVector(const LPARAM &lparam)
	{
		return Vec2i(
			GET_X_LPARAM(lparam),
			GET_Y_LPARAM(lparam));
	}

	/// to Win32
	POINT MakePoint(const Vector<2, LONG> &v)
	{
		POINT pt = {v.x, v.y};
		return pt;
	}

	RECT MakeRect(const Aabb<2, LONG> &a)
	{
		RECT rect =
		{
			a.min.x, a.min.y,
			a.max.x, a.max.y
		};
		return rect;
	}
}}}
