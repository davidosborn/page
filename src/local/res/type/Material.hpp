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

#ifndef    page_local_res_type_Material_hpp
#   define page_local_res_type_Material_hpp

#	include <vector>

#	include "../../cache/proxy/ResourceProxy.hpp"
#	include "../../math/Color.hpp" // Rgb{,a}Color
#	include "../../math/Vector.hpp"

namespace page
{
	namespace res
	{
		class Image;

		struct Material
		{
			struct Pass
			{
				struct Texture
				{
					Texture();
					Texture(const cache::ResourceProxy<Image> &image);

					cache::ResourceProxy<Image> image;
					math::Vec2 offset, scale;
					unsigned uvIndex; // break out the sunscreen!
				};
				struct Power
				{
					Power();

					float value;
					Texture texture;
				};
				struct Ambient
				{
					math::RgbColor<> color;
					Texture texture;
				} ambient;
				struct Diffuse
				{
					Diffuse();

					math::RgbaColor<> color;
					Texture texture;
				} diffuse;
				struct Emissive
				{
					math::RgbColor<> color;
					Texture texture;
				} emissive;
				struct Fresnel
				{
					Fresnel();

					math::RgbColor<> color;
					Texture texture;
					Power power;
				} fresnel;
				struct Gloss
				{
					Gloss();

					float value;
					Texture texture;
				} gloss;
				struct Mask
				{
					Mask();

					float value;
					Texture texture;
				} mask;
				struct Normal
				{
					Texture texture;
				} normal;
				struct Specular
				{
					math::RgbColor<> color;
					Texture texture;
					Power power;
				} specular;
			};
			typedef std::vector<Pass> Passes;
			Passes passes;
		};
	}
}

#endif
