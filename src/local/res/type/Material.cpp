#include "Material.hpp"
#include "Registry.hpp" // REGISTER_TYPE

namespace page
{
	namespace res
	{
		// material pass texture construct
		Material::Pass::Texture::Texture() : scale(1), uvIndex(0) {}
		Material::Pass::Texture::Texture(const cache::Resource<Image> &image) :
			image(image), scale(1), uvIndex(0) {}

		// material pass miscellaneous construct
		Material::Pass::Power::Power() : value(0) {}
		Material::Pass::Diffuse::Diffuse() : color(1) {}
		Material::Pass::Fresnel::Fresnel() : color(1) {}
		Material::Pass::Gloss::Gloss() : value(0) {}
		Material::Pass::Mask::Mask() : value(1) {}

		REGISTER_TYPE(Material, "material", 0)
	}
}
