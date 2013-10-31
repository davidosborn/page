#ifndef    page_local_phys_node_Emitter_hpp
#   define page_local_phys_node_Emitter_hpp

#	include <string>

#	include "../attrib/AmbientRange.hpp"
#	include "../attrib/Cutoff.hpp"
#	include "../attrib/DiffuseRange.hpp"
#	include "../attrib/EmissiveRange.hpp"
#	include "../attrib/LifetimeRange.hpp"
#	include "../attrib/Material.hpp"
#	include "../attrib/OpacityRange.hpp"
#	include "../attrib/PositionOrientation.hpp"
#	include "../attrib/SizeRange.hpp"
#	include "../attrib/SpecularRange.hpp"
#	include "../attrib/SpeedRange.hpp"
#	include "../mixin/Controllable.hpp"
#	include "Node.hpp"

namespace page { namespace phys
{
	/**
	 * A particle emitter.
	 */
	class Emitter :
		public Node,
		public Controllable,
		public attrib::AmbientRange,
		public attrib::Cutoff,
		public attrib::DiffuseRange,
		public attrib::EmissiveRange,
		public attrib::LifetimeRange,
		public attrib::Material,
		public attrib::OpacityRange,
		public attrib::PositionOrientation,
		public attrib::SpecularRange,
		public attrib::SizeRange,
		public attrib::SpeedRange
	{
		IMPLEMENT_CLONEABLE(Emitter, Node)

		public:
		// update
		void Update();

		/*--------------------+
		| frame serialization |
		+--------------------*/

		protected:
		Frame GetFrame() const override;
		void SetFrame(const Frame &) override;
	};
}}

#endif
