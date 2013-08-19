#ifndef    page_local_vid_opengl_Texture_hpp
#   define page_local_vid_opengl_Texture_hpp

#	include <GL/gl.h> // GLuint

#	include "../../math/fwd.hpp" // RgbaColor
#	include "../../math/Vector.hpp"
#	include "../../util/class/special_member_functions.hpp" // Uncopyable
#	include "TextureFlags.hpp"
#	include "TextureFormat.hpp"

namespace page
{
	namespace res { class Image; }

	namespace vid
	{
		namespace opengl
		{
			struct Texture : util::Uncopyable<Texture>
			{
				typedef TextureFlags  Flags;
				typedef TextureFormat Format;

				// construct/destroy
				explicit Texture(const math::RgbaColor<> &);
				explicit Texture(const res::Image &,
					Format = defaultTextureFormat,
					Flags = static_cast<Flags>(filterTextureFlag | mipmapTextureFlag),
					const math::Vector<2, bool> &clamp = false);
				~Texture();

				// attributes
				const math::Vec2u &GetSize() const;
				const math::Vec2u &GetPow2Size() const;

				// handle access
				GLuint GetHandle() const;

				private:
				GLuint handle;
				math::Vec2u size, pow2Size;
			};

			// binding
			void Bind(const Texture &);
		}
	}
}

#endif
