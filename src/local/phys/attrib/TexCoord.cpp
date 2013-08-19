#include "TexCoord.hpp"

namespace page
{
	namespace phys
	{
		namespace attrib
		{
			// construct
			TexCoord::TexCoord(const math::Vec2 &texCoord) :
				texCoord(texCoord) {}

			// access
			const math::Vec2 &TexCoord::GetTexCoord() const
			{
				return texCoord;
			}
			void TexCoord::SetTexCoord(const math::Vec2 &texCoord)
			{
				this->texCoord = texCoord;
			}

			// frame serialization
			Frame TexCoord::GetFrame() const
			{
				Frame frame;
				frame.texCoord = texCoord;
				return frame;
			}
			void TexCoord::Update(const Frame &frame)
			{
				if (frame.texCoord) texCoord = *frame.texCoord;
			}
		}
	}
}
