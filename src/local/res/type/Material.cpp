/**
 * @copyright
 *
 * Copyright (c) 2006-2014 David Osborn
 *
 * Permission is granted to use and redistribute this software in source and
 * binary form, with or without modification, subject to the following
 * conditions:
 *
 * 1. Redistributions in source form must retain the above copyright notice,
 *    this list of conditions, and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions, and the following disclaimer in the same place
 *    and form as other copyright, license, and disclaimer information.
 *
 * 3. Redistributions in binary form must also include an acknowledgement in the
 *    same place and form as other acknowledgements (such as the credits),
 *    similar in substance to the following:
 *
 *       Portions of this software are based on the work of David Osborn.
 *
 * This software is provided "as is", without any express or implied warranty.
 * In no event will the authors be liable for any damages arising out of the use
 * of this software.
 */

#include "Material.hpp"
#include "Registry.hpp" // REGISTER_TYPE

namespace page
{
	namespace res
	{
		// material pass texture construct
		Material::Pass::Texture::Texture() : scale(1), uvIndex(0) {}
		Material::Pass::Texture::Texture(const cache::ResourceProxy<Image> &image) :
			image(image), scale(1), uvIndex(0) {}

		// material pass miscellaneous construct
		Material::Pass::Power::Power() : value(0) {}
		Material::Pass::Diffuse::Diffuse() : color(1) {}
		Material::Pass::Fresnel::Fresnel() : color(1) {}
		Material::Pass::Gloss::Gloss() : value(0) {}
		Material::Pass::Mask::Mask() : value(1) {}

		REGISTER_TYPE(Material, "material")
	}
}
