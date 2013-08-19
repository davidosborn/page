#ifndef    page_local_vid_opengl_FontTexture_hpp
#   define page_local_vid_opengl_FontTexture_hpp

#	include <unordered_map>

#	include <GL/gl.h> // GLuint

#	include "../../math/Aabb.hpp"

namespace page
{
	namespace res { class Font; }

	namespace vid
	{
		namespace opengl
		{
			struct FontTexture
			{
				// construct/destroy
				FontTexture(const res::Font &, unsigned fontSize);
				~FontTexture();

				// sections
				const math::Aabb<2> *GetSection(char) const;

				// handle access
				GLuint GetHandle() const;

				private:
				mutable GLuint handle;
				typedef std::unordered_map<char, math::Aabb<2>> Sections;
				Sections sections;
			};

			// binding
			void Bind(const FontTexture &);
		}
	}
}

#endif
