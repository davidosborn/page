#ifndef    page_local_res_type_Model_hpp
#   define page_local_res_type_Model_hpp

#	include <string>
#	include <vector>

#	include "../../cache/proxy/ResourceProxy.hpp"
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
			cache::ResourceProxy<Mesh> mesh;
			cache::ResourceProxy<Material> material;
		};
		typedef std::vector<Part> Parts;
		Parts parts;
		cache::ResourceProxy<Skeleton> skeleton;
	};

	/**
	 * @return An array of geometrically-unique meshes.
	 */
	util::reference_vector<cache::Proxy<res::Mesh>>
		GetGeometricallyDistinctMeshes(const Model &);
}}

#endif
