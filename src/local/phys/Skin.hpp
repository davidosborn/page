// binding between mesh and pose

#ifndef    page_local_phys_Skin_hpp
#   define page_local_phys_Skin_hpp

#	include <vector>

#	include "../math/Vector.hpp"
#	include "attrib/Pose.hpp" // Pose::Bone

namespace page
{
	namespace res { class Mesh; }

	namespace phys
	{
		struct Skin
		{
			// construct
			Skin(const res::Mesh &mesh, const attrib::Pose &);

			struct Vertex
			{
				math::Vec3 co, no;
				struct Influence
				{
					const attrib::Pose::Bone *bone;
					float weight;
				};
				typedef std::vector<Influence> Influences;
				Influences influences;
			};
			typedef std::vector<Vertex> Vertices;
			Vertices vertices;
		};

		// transform vertex
		void Update(const Skin::Vertex &, math::Vec3 &co, math::Vec3 &no);
	}
}

#endif
