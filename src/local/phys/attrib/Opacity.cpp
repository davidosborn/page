#include "Opacity.hpp"

namespace page
{
	namespace phys
	{
		namespace attrib
		{
			// construct
			Opacity::Opacity(float opacity) : opacity(opacity) {}

			// access
			float Opacity::GetOpacity() const
			{
				return opacity;
			}
			void Opacity::SetOpacity(float opacity)
			{
				this->opacity = opacity;
			}

			// frame serialization
			Frame Opacity::GetFrame() const
			{
				Frame frame;
				frame.opacity = opacity;
				return frame;
			}
			void Opacity::Update(const Frame &frame)
			{
				if (frame.opacity) opacity = *frame.opacity;
			}
		}
	}
}
