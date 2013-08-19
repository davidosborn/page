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
