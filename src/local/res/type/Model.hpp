#ifndef    page_local_res_type_Model_hpp
#   define page_local_res_type_Model_hpp

#	include <string>
#	include <vector>

#	include "../../cache/proxy/Resource.hpp"
#	include "../../util/container/reference_vector.hpp"

namespace page { namespace res
{
	class Material;
	class Mesh;
	class Skeleton;

	/**
	 *
	 */
	struct Model
	{
		struct Part
		{
			std::string name;
			cache::Resource<Mesh> mesh;
			cache::Resource<Material> material;
		};
		typedef std::vector<Part> Parts;
		Parts parts;
		cache::Resource<Skeleton> skeleton;
	};

	/**
	 * @return An array of geometrically-unique meshes.
	 */
	util::reference_vector<cache::Proxy<res::Mesh>>
		GetGeometricallyDistinctMeshes(const Model &);
}}

#endif
