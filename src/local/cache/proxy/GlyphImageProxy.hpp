#ifndef    page_local_cache_proxy_GlyphImageProxy_hpp
#   define page_local_cache_proxy_GlyphImageProxy_hpp

#	include "BasicProxy.hpp"
#	include "Proxy.hpp"

namespace page { namespace res
{
	class Font;
	class Image;
}}

namespace page { namespace cache
{
	/**
	 * A proxy representing a glyph image in the cache.
	 */
	class GlyphImageProxy :
		public BasicProxy<res::Image>,
		public virtual util::Cloneable<GlyphImageProxy, BasicProxy<res::Image>>
	{
		/*-------+
		| traits |
		+-------*/

		public:
		using BasicProxy<res::Image>::pointer;

		/*-------------+
		| constructors |
		+-------------*/

		GlyphImageProxy(const Proxy<res::Font> &, char ch, unsigned size);

		/*--------------------------+
		| BasicProxy implementation |
		+--------------------------*/

		private:
		pointer DoLock() const override;

		/*-------------+
		| data members |
		+-------------*/

		Proxy<res::Font> font;
		char ch;
		unsigned size;
	};
}}

#endif
