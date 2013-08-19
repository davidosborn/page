#include "Exposure.hpp"

namespace page
{
	namespace phys
	{
		namespace attrib
		{
			// construct
			Exposure::Exposure(float exposure) : exposure(exposure) {}

			// access
			float Exposure::GetExposure() const
			{
				return exposure;
			}
			void Exposure::SetExposure(float exposure)
			{
				this->exposure = exposure;
			}

			// frame serialization
			Frame Exposure::GetFrame() const
			{
				Frame frame;
				frame.exposure = exposure;
				return frame;
			}
			void Exposure::Update(const Frame &frame)
			{
				if (frame.exposure) exposure = *frame.exposure;
			}
		}
	}
}
