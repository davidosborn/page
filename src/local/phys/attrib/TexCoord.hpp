#ifndef    page_local_phys_attrib_TexCoord_hpp
#   define page_local_phys_attrib_TexCoord_hpp

#	include "../../math/Vector.hpp"
#	include "../Frame.hpp"

namespace page { namespace phys { namespace attrib
{
	class TexCoord
	{
		/*-------------+
		| constructors |
		+-------------*/

		public:
		explicit TexCoord(const math::Vec2 & = 0);

		/*----------+
		| accessors |
		+----------*/

		const math::Vec2 &GetTexCoord() const;
		void SetTexCoord(const math::Vec2 &);

		/*--------------------+
		| frame serialization |
		+--------------------*/

		protected:
		Frame GetFrame() const;
		void SetFrame(const Frame &);

		/*-------------+
		| data members |
		+-------------*/

		private:
		math::Vec2 value;
	};
}}}

#endif
