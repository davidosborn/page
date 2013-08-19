#include "Attenuation.hpp"

namespace page
{
	namespace phys
	{
		namespace attrib
		{
			// construct
			Attenuation::Attenuation(float attenuation) :
				attenuation(attenuation) {}

			// access
			float Attenuation::GetAttenuation() const
			{
				return attenuation;
			}
			void Attenuation::SetAttenuation(float attenuation)
			{
				this->attenuation = attenuation;
			}

			// frame serialization
			Frame Attenuation::GetFrame() const
			{
				Frame frame;
				frame.attenuation = attenuation;
				return frame;
			}
			void Attenuation::Update(const Frame &frame)
			{
				if (frame.attenuation) attenuation = *frame.attenuation;
			}
		}
	}
}
