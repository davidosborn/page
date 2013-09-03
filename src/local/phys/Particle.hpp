#ifndef    page_local_phys_Particle_hpp
#   define page_local_phys_Particle_hpp

#	include "../cache/proxy/fwd.hpp" // Proxy
#	include "attrib/Ambient.hpp"
#	include "attrib/Diffuse.hpp"
#	include "attrib/Emissive.hpp"
#	include "attrib/Material.hpp"
#	include "attrib/Opacity.hpp"
#	include "attrib/Position.hpp"
#	include "attrib/Size.hpp"
#	include "attrib/Specular.hpp"

namespace page
{
	namespace res { class Material; }

	namespace phys
	{
		struct Particle :
			attrib::Ambient,
			attrib::Diffuse,
			attrib::Emissive,
			attrib::Material,
			attrib::Position,
			attrib::Opacity,
			attrib::Size,
			attrib::Specular
		{
			// construct
			explicit Particle(const cache::Proxy<res::Material> &material);

			protected:
			// frame serialization
			// FIXME: should Particle be Controllable?
			/*Frame GetFrame() const;
			void Update(const Frame &);*/

			private:
			float lifetime, drag, mass;
		};
	}
}

#endif
