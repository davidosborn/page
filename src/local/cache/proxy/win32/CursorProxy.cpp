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

#include "../../../res/type/cursor/win32.hpp" // MakeWin32Cursor
#include "CursorProxy.hpp"

namespace page { namespace cache { namespace win32
{
	namespace
	{
		/**
		 *
		 */
		struct CursorDeleter
		{
			typedef void result_type;
			typedef const HCURSOR *argument_type;

			void operator ()(const HCURSOR *cursor) const
			{
				DestroyCursor(*cursor);
				delete cursor;
			}
		};
	}

	/*-------------+
	| constructors |
	+-------------*/

	CursorProxy::CursorProxy(const Proxy<res::Cursor> &cursor, unsigned size) :
		BasicProxy<HCURSOR>(Signature("Win32 cursor", cursor, size)),
		cursor(cursor), size(size) {}

	/*--------------------------+
	| BasicProxy implementation |
	+--------------------------*/

	auto CursorProxy::DoLock() const -> pointer
	{
		return pointer(new HCURSOR(res::win32::MakeWin32Cursor(*cursor, size)), CursorDeleter());
	}
}}}
