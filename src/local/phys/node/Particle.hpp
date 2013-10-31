#ifndef    page_local_phys_node_Particle_hpp
#   define page_local_phys_node_Particle_hpp

#	include "../../cache/proxy/fwd.hpp" // Proxy
#	include "../attrib/Ambient.hpp"
#	include "../attrib/Diffuse.hpp"
#	include "../attrib/Emissive.hpp"
#	include "../attrib/Material.hpp"
#	include "../attrib/Opacity.hpp"
#	include "../attrib/Position.hpp"
#	include "../attrib/Size.hpp"
#	include "../attrib/Specular.hpp"
#	include "Node.hpp"

namespace page { namespace res { class Material; }}

namespace page { namespace phys
{
	class Particle :
		public Node,
		public attrib::Ambient,
		public attrib::Diffuse,
		public attrib::Emissive,
		public attrib::Material,
		public attrib::Position,
		public attrib::Opacity,
		public attrib::Size,
		public attrib::Specular
	{
		IMPLEMENT_CLONEABLE(Particle, Node)

		// construct
		explicit Particle(const cache::Proxy<res::Material> &material);

		/*--------------------+
		| frame serialization |
		+--------------------*/

		protected:
		Frame GetFrame() const override;
		void SetFrame(const Frame &) override;

		private:
		float lifetime, drag, mass;
	};
}}

#endif
