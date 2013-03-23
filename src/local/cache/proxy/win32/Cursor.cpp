/**
 * @section license
 *
 * Copyright (c) 2006-2012 David Osborn
 *
 * Permission is granted to use and redistribute this software in source and
 * binary form, with or without modification, subject to the following
 * conditions:
 *
 * 1. Redistributions in source form must retain the above copyright notice,
 *    this list of conditions, and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions, and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution, and in the same
 *    place and form as other copyright, license, and disclaimer information.
 *
 * As a special exception, distributions of derivative works in binary form may
 * include an acknowledgement in place of the above copyright notice, this list
 * of conditions, and the following disclaimer in the documentation and/or other
 * materials provided with the distribution, and in the same place and form as
 * other acknowledgements, similar in substance to the following:
 *
 *    Portions of this software are based on the work of David Osborn.
 *
 * This software is provided "as is", without any express or implied warranty.
 * In no event will the authors be liable for any damages arising out of the use
 * of this software.
 */

#include "../../../res/type/cursor/win32.hpp" // MakeWin32Cursor
#include "../../../util/string/StringBuilder.hpp"
#include "Cursor.hpp"

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

////////// Cursor //////////////////////////////////////////////////////////////

	/*--------------------------+
	| constructors & destructor |
	+--------------------------*/

	Cursor::Cursor(const Proxy<res::Cursor> &cursor, unsigned size) :
		cursor(cursor.Copy()), size(size) {}

	/*----------+
	| observers |
	+----------*/

	std::string Cursor::GetType() const
	{
		return "win32 cursor";
	}

	std::string Cursor::GetSource() const
	{
		return util::StringBuilder() <<
			cursor->GetSource() << ':' << size;
	}

	Cursor::operator bool() const
	{
		return *cursor;
	}

	/*--------------+
	| instantiation |
	+--------------*/

	Cursor::Instance Cursor::Make() const
	{
		return Instance(new HCURSOR(res::win32::MakeWin32Cursor(**cursor, size)), CursorDeleter());
	}
}}}
