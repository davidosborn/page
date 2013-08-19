#ifndef    page_local_cache_proxy_win32_Cursor_hpp
#   define page_local_cache_proxy_win32_Cursor_hpp

#	include <windows.h> // HCURSOR

#	include "../Proxy.hpp"

namespace page { namespace res { class Cursor; }}

namespace page { namespace cache { namespace win32
{
	/**
	 * A proxy representing a cursor in the cache.
	 */
	class Cursor :
		public Proxy<HCURSOR>,
		public virtual util::Cloneable<Cursor, Proxy<HCURSOR>>
	{
		/*------+
		| types |
		+------*/

		public:
		typedef typename Proxy<HCURSOR>::Instance Instance;

		/*--------------------------+
		| constructors & destructor |
		+--------------------------*/

		public:
		explicit Cursor(const Proxy<res::Cursor> &, unsigned size);

		/*----------+
		| observers |
		+----------*/

		public:
		std::string GetType() const override;
		std::string GetSource() const override;
		operator bool() const override;

		/*--------------+
		| instantiation |
		+--------------*/

		private:
		Instance Make() const override;

		/*-------------+
		| data members |
		+-------------*/

		private:
		Proxy<res::Cursor> cursor;
		unsigned size;
	};
}}}

#endif
