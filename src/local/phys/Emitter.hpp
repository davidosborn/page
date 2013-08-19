#ifndef    page_local_phys_Emitter_hpp
#   define page_local_phys_Emitter_hpp

#	include <string>

#	include "attrib/AmbientRange.hpp"
#	include "attrib/Cutoff.hpp"
#	include "attrib/DiffuseRange.hpp"
#	include "attrib/EmissiveRange.hpp"
#	include "attrib/LifetimeRange.hpp"
#	include "attrib/Material.hpp"
#	include "attrib/OpacityRange.hpp"
#	include "attrib/PositionOrientation.hpp"
#	include "attrib/SizeRange.hpp"
#	include "attrib/SpecularRange.hpp"
#	include "attrib/SpeedRange.hpp"
#	include "mixin/Controllable.hpp"

namespace page
{
	namespace phys
	{
		struct Emitter : Controllable,
			attrib::AmbientRange,
			attrib::Cutoff,
			attrib::DiffuseRange,
			attrib::EmissiveRange,
			attrib::LifetimeRange,
			attrib::Material,
			attrib::OpacityRange,
			attrib::PositionOrientation,
			attrib::SpecularRange,
			attrib::SizeRange,
			attrib::SpeedRange
		{
			// update
			void Update();

			protected:
			// frame serialization
			Frame GetFrame() const;
			void Update(const Frame &);
		};
	}
}

#endif
