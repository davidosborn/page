#include "Falloff.hpp"

namespace page
{
	namespace phys
	{
		namespace attrib
		{
			// construct
			Falloff::Falloff(float falloff) : falloff(falloff) {}

			// access
			float Falloff::GetFalloff() const
			{
				return falloff;
			}
			void Falloff::SetFalloff(float falloff)
			{
				this->falloff = falloff;
			}

			// frame serialization
			Frame Falloff::GetFrame() const
			{
				Frame frame;
				frame.falloff = falloff;
				return frame;
			}
			void Falloff::Update(const Frame &frame)
			{
				if (frame.falloff) falloff = *frame.falloff;
			}
		}
	}
}
