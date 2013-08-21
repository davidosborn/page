#ifndef    page_local_cache_proxy_win32_CursorProxy_hpp
#   define page_local_cache_proxy_win32_CursorProxy_hpp

#	include <windows.h> // HCURSOR

#	include "../BasicProxy.hpp"
#	include "../Proxy.hpp"

namespace page { namespace res { class Cursor; }}

namespace page { namespace cache { namespace win32
{
	/**
	 * A proxy representing a cursor in the cache.
	 */
	class CursorProxy :
		public BasicProxy<HCURSOR>,
		public virtual util::Cloneable<CursorProxy, BasicProxy<HCURSOR>>
	{
		/*-------+
		| traits |
		+-------*/

		public:
		using typename BasicProxy<HCURSOR>::pointer;

		/*-------------+
		| constructors |
		+-------------*/

		explicit Cursor(const Proxy<res::Cursor> &, unsigned size);

		/*--------------------------+
		| BasicProxy implementation |
		+--------------------------*/

		private:
		pointer DoLock() const override;

		/*-------------+
		| data members |
		+-------------*/

		Proxy<res::Cursor> cursor;
		unsigned size;
	};
}}}

#endif
