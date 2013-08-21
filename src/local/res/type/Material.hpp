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
