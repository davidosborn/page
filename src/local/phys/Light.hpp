#ifndef    page_local_phys_Light_hpp
#   define page_local_phys_Light_hpp

#	include "attrib/Ambient.hpp"
#	include "attrib/Attenuation.hpp"
#	include "attrib/Cutoff.hpp"
#	include "attrib/Diffuse.hpp"
#	include "attrib/Falloff.hpp"
#	include "attrib/PositionOrientation.hpp"
#	include "attrib/Range.hpp"
#	include "attrib/Specular.hpp"
#	include "mixin/Controllable.hpp"

namespace page
{
	namespace phys
	{
		struct Light : Controllable,
			attrib::Ambient,
			attrib::Attenuation,
			attrib::Cutoff,
			attrib::Diffuse,
			attrib::Falloff,
			attrib::PositionOrientation,
			attrib::Range,
			attrib::Specular
		{
			// attenuation state
			float GetMinRange() const;
			float GetMaxRange() const;

			protected:
			// frame serialization
			Frame GetFrame() const;
			void Update(const Frame &);
		};
	}
}

#endif
