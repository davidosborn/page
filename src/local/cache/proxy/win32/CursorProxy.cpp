#include "../../../res/type/cursor/win32.hpp" // MakeWin32Cursor
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
		return pointer(new HCURSOR(res::win32::MakeWin32Cursor(**cursor, size)), CursorDeleter());
	}
}}}
