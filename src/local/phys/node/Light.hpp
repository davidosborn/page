#ifndef    page_local_phys_node_Light_hpp
#   define page_local_phys_node_Light_hpp

#	include "../attrib/Ambient.hpp"
#	include "../attrib/Attenuation.hpp"
#	include "../attrib/Cutoff.hpp"
#	include "../attrib/Diffuse.hpp"
#	include "../attrib/Falloff.hpp"
#	include "../attrib/PositionOrientation.hpp"
#	include "../attrib/Range.hpp"
#	include "../attrib/Specular.hpp"
#	include "../mixin/Controllable.hpp"
#	include "Node.hpp"

namespace page { namespace phys
{
	class Light :
		public Node,
		public Controllable,
		public attrib::Ambient,
		public attrib::Attenuation,
		public attrib::Cutoff,
		public attrib::Diffuse,
		public attrib::Falloff,
		public attrib::PositionOrientation,
		public attrib::Range,
		public attrib::Specular
	{
		IMPLEMENT_CLONEABLE(Light, Node)

		public:
		// attenuation state
		float GetMinRange() const;
		float GetMaxRange() const;

		/*--------------------+
		| frame serialization |
		+--------------------*/

		protected:
		Frame GetFrame() const override;
		void SetFrame(const Frame &) override;
	};
}}

#endif
