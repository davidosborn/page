#ifndef    page_local_phys_attrib_TexCoord_hpp
#   define page_local_phys_attrib_TexCoord_hpp

#	include "../../math/Vector.hpp"
#	include "../Frame.hpp"

namespace page
{
	namespace phys
	{
		namespace attrib
		{
			struct TexCoord
			{
				// construct
				explicit TexCoord(const math::Vec2 & = 0);

				// access
				const math::Vec2 &GetTexCoord() const;
				void SetTexCoord(const math::Vec2 &);

				protected:
				// frame serialization
				Frame GetFrame() const;
				void Update(const Frame &);

				private:
				math::Vec2 texCoord;
			};
		}
	}
}

#endif
