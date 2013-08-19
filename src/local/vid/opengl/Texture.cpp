#include <algorithm> // transform
#include <cassert>

#include "../../cfg/vars.hpp"
#include "../../math/Color.hpp" // RgbaColor
#include "../../math/pow2.hpp" // Pow2Ceil
#include "ext.hpp" // ARB_texture_non_power_of_two
#include "tex.hpp" // Make{,Alpha,Luminance}Texture
#include "Texture.hpp"

namespace page
{
	namespace vid
	{
		namespace opengl
		{
			// construct/destroy
			Texture::Texture(const math::RgbaColor<> &color) :
				size(1), pow2Size(1)
			{
				glGenTextures(1, &handle);
				glBindTexture(GL_TEXTURE_2D, handle);
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1, 1, 0, GL_RGBA,
					GL_FLOAT, &*math::RgbaColor<GLfloat>(color).begin());
			}
			Texture::Texture(const res::Image &img, Format format, Flags flags, const math::Vector<2, bool> &clamp) :
				size(img.size), pow2Size(Max(size >> *CVAR(opengl)::renderTextureDown, 1))
			{
				// FIXME: texture size calculations are duplicated in tex.cpp
				if (!haveArbTextureNonPowerOfTwo)
					std::transform(size.begin(), size.end(), pow2Size.begin(), math::Pow2Ceil);
				// FIXME: Make*Texture should accept flags directly
				bool mipmap = flags & mipmapTextureFlag;
				switch (format)
				{
					case defaultTextureFormat:   handle = MakeTexture(         img, mipmap, clamp); break;
					case alphaTextureFormat:     handle = MakeAlphaTexture(    img, mipmap, clamp); break;
					case luminanceTextureFormat: handle = MakeLuminanceTexture(img, mipmap, clamp); break;
					default: assert(!"invalid texture format");
				}
			}
			Texture::~Texture()
			{
				glDeleteTextures(1, &handle);
			}

			// attributes
			const math::Vec2u &Texture::GetSize() const
			{
				return size;
			}
			const math::Vec2u &Texture::GetPow2Size() const
			{
				return pow2Size;
			}

			// handle access
			GLuint Texture::GetHandle() const
			{
				return handle;
			}

			// binding
			void Bind(const Texture &texture)
			{
				glBindTexture(GL_TEXTURE_2D, texture.GetHandle());
				glEnable(GL_TEXTURE_2D);
			}
		}
	}
}
