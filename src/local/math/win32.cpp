/**
 * @copyright
 *
 * Copyright (c) 2006-2014 David Osborn
 *
 * Permission is granted to use and redistribute this software in source and
 * binary form, with or without modification, subject to the following
 * conditions:
 *
 * 1. Redistributions in source form must retain the above copyright notice,
 *    this list of conditions, and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions, and the following disclaimer in the same place
 *    and form as other copyright, license, and disclaimer information.
 *
 * 3. Redistributions in binary form must also include an acknowledgement in the
 *    same place and form as other acknowledgements (such as the credits),
 *    similar in substance to the following:
 *
 *       Portions of this software are based on the work of David Osborn.
 *
 * This software is provided "as is", without any express or implied warranty.
 * In no event will the authors be liable for any damages arising out of the use
 * of this software.
 */

#include <windows.h> // LONG, POINT, RECT
#include <windowsx.h> // GET_[XY]_LPARAM

#include "Aabb.hpp"
#include "Vector.hpp"

namespace page { namespace math { namespace win32
{
	/*-----------+
	| from Win32 |
	+-----------*/

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

	/*---------+
	| to Win32 |
	+---------*/

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
