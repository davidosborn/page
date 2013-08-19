#include <algorithm> // max, min
#include "Fov.hpp"

namespace page
{
	namespace phys
	{
		namespace attrib
		{
			// construct
			Fov::Fov(float fov) : fov(fov) {}

			// access
			float Fov::GetFov() const
			{
				return fov;
			}
			void Fov::SetFov(float fov)
			{
				this->fov = std::min(std::max(fov, 0.f), 180.f);
			}

			// frame serialization
			Frame Fov::GetFrame() const
			{
				Frame frame;
				frame.fov = fov;
				return frame;
			}
			void Fov::Update(const Frame &frame)
			{
				if (frame.fov) fov = *frame.fov;
			}
		}
	}
}
